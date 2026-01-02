using UnityEngine;

public class DynamicFloater : MonoBehaviour
{
    #region REGION: Public Properties
    [Header("Movement Settings")]
    public float MaximalForwardSpeed = 10f;
    public float MaximalBackwardSpeed = 5f;
    public float Acceleration = 5f;
    public float TurnSpeed = 50f;

    [Header("Autonomous Inputs (-1 to 1)")]
    public bool RandomizeTurn = false;
    [Range(-1f, 1f)] public float ForwardInput; // Forward / backward control
    [Range(-1f, 1f)] public float TurnInput;    // Left / right turn control

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
    #endregion

    private void Start()
    {
        _CurrentYaw = transform.eulerAngles.y;
        if (RandomizeTurn) 
            TurnInput = Random.Range(-1.0f, 1.0f);
    }

    private void Update()
    {
        UpdateSpeed();
        UpdateYaw();
        UpdateMovement();
        UpdateRotation();
    }

    void UpdateSpeed()
    {
        // Determine target speed based on forward input.
        float target_speed = 0f;

        if (ForwardInput > 0f)
            target_speed = ForwardInput * MaximalForwardSpeed;
        else if (ForwardInput < 0f)
            target_speed = ForwardInput * MaximalBackwardSpeed;

        // Smoothly move current speed toward the target speed.
        _CurrentSpeed = Mathf.MoveTowards(
            _CurrentSpeed,
            target_speed,
            Acceleration * Time.deltaTime
        );
    }

    void UpdateYaw()
    {
        // Apply yaw rotation based on turn input and current speed.
        _CurrentYaw += TurnInput * TurnSpeed * Time.deltaTime;
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
