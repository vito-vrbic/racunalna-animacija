using UnityEngine;

public class BoatController : MonoBehaviour
{
    #region REGION: Public Properties
    [Header("Movement Settings")]
    public float MaximalForwardSpeed = 10f;
    public float MaximalBackwardSpeed = 5f;
    public float Acceleration = 5f;
    public float TurnSpeed = 50f;

    [Header("Tilt Settings")]
    public float PitchStrength = 25f;
    public float RollStrength = 30f;
    public float RotationSmooth = 6f;

    [Header("Height Samplers")]
    public Transform FrontSampler;
    public Transform BackSampler;
    public Transform LeftSampler;
    public Transform RightSampler;
    #endregion

    #region REGION: Private Fields
    private float _CurrentSpeed;
    private float _CurrentYaw;
    private Rigidbody _RB;
    #endregion

    void Awake()
    {
        _RB = GetComponent<Rigidbody>();
    }

    void Update()
    {
        UpdateSpeed();
        UpdateYaw();
        UpdateMovement();
        UpdateRotation();

        if (Input.GetKey(KeyCode.Escape)) Application.Quit();
    }

    private void FixedUpdate()
    {
        _RB.velocity = Vector3.zero;
        _RB.angularVelocity = Vector3.zero;
    }

    void UpdateSpeed() 
    {
        float forward_input = Input.GetAxis("Vertical");

        float target_speed = 0f;

        if (forward_input > 0f)
            target_speed = forward_input * MaximalForwardSpeed;
        else if (forward_input < 0f)
            target_speed = forward_input * MaximalBackwardSpeed;

        _CurrentSpeed = Mathf.MoveTowards(
            _CurrentSpeed,
            target_speed,
            Acceleration * Time.deltaTime
        );
    }

    void UpdateYaw() 
    {
        float turn_input = Input.GetAxis("Horizontal");
        _CurrentYaw += turn_input * TurnSpeed * (Mathf.Abs(_CurrentSpeed) / MaximalForwardSpeed) * Time.deltaTime;
    }

    void UpdateMovement() 
    {
        // Move forward
        Vector3 move_dir = Quaternion.Euler(0f, _CurrentYaw, 0f) * Vector3.forward;
        transform.position += move_dir * _CurrentSpeed * Time.deltaTime;
    }

    void UpdateRotation()
    {
        if (!WaterSurface.Instance) return;
        if (!FrontSampler || !BackSampler || !LeftSampler || !RightSampler) return;

        // Sample water height at sampler transforms.
        float front_y = WaterSurface.Instance.GetHeightAt(FrontSampler.position);
        float back_y = WaterSurface.Instance.GetHeightAt(BackSampler.position);
        float left_y = WaterSurface.Instance.GetHeightAt(LeftSampler.position);
        float right_y = WaterSurface.Instance.GetHeightAt(RightSampler.position);

        // Set the height of the object to the average value of sampling.
        Vector3 target = transform.position;
        target.y = (front_y + back_y + left_y + right_y) * 0.25f;
        transform.position = target;

        // Compute the pitch and roll, then clamp them.
        float pitch = (back_y - front_y) * PitchStrength;
        float roll = (right_y - left_y) * RollStrength;
        pitch = Mathf.Clamp(pitch, -20f, 20f);
        roll = Mathf.Clamp(roll, -25f, 25f);

        // Compute rotation using Euler angles.
        Quaternion target_rotation = Quaternion.Euler(pitch, _CurrentYaw, roll);

        // SLerp towards the desired rotation.
        transform.rotation = Quaternion.Slerp(
            transform.rotation,
            target_rotation,
            Time.deltaTime * RotationSmooth
        );
    }
}