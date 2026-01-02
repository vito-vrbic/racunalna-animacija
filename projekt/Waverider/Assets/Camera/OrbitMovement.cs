using UnityEngine;

public class OrbitMovement : MonoBehaviour
{
    #region REGION: Public Properties
    [Header("Target Settings")]
    public Transform Target;
    public Vector3 PositionOffset = new Vector3(0f, 5f, -10f);

    [Header("Orbit Settings")]
    public float RotationSpeed = 5f;
    public float ZoomSpeed = 5f;
    public float MinimalZoom = 5f;
    public float MaximalZoom = 20f;

    [Header("Water Safety")]
    public float MinimalHeightAboveWater = 2f;
    #endregion

    #region REGION: Private Fields
    private float _CurrentZoom;
    private float _Yaw;
    private float _Pitch;
    private bool _IsRotating = false;
    #endregion

    void Start()
    {
        if (!Target)
        {
            Debug.LogError("OrbitMovement: No target object assigned!");
            enabled = false;
            return;
        }

        // Set initial values.
        _CurrentZoom = PositionOffset.magnitude;
        _Yaw = transform.eulerAngles.y;
        _Pitch = transform.eulerAngles.x;

        // Make cursor visible initially.
        Cursor.visible = true;
        Cursor.lockState = CursorLockMode.None;
    }

    void LateUpdate()
    {
        if (!Target) return;

        HandleMouseInput();

        ApplyOrbitRotation();
    }

    void HandleMouseInput() 
    {
        if (Input.GetMouseButtonDown(1))
        {
            _IsRotating = true;
            Cursor.lockState = CursorLockMode.Locked;
            Cursor.visible = false;
        }
        else if (Input.GetMouseButtonUp(1))
        {
            _IsRotating = false;
            Cursor.lockState = CursorLockMode.None;
            Cursor.visible = true;
        }

        // Handle rotation for mouse movement if needed.
        if (_IsRotating)
        {
            _Yaw += Input.GetAxis("Mouse X") * RotationSpeed;
            _Pitch -= Input.GetAxis("Mouse Y") * RotationSpeed;
            _Pitch = Mathf.Clamp(_Pitch, 10f, 80f);
        }

        // Handle zooming.
        float scroll = Input.GetAxis("Mouse ScrollWheel");
        _CurrentZoom -= scroll * ZoomSpeed;
        _CurrentZoom = Mathf.Clamp(_CurrentZoom, MinimalZoom, MaximalZoom);
    }

    void ApplyOrbitRotation() 
    {
        // Calculate the target's new position.
        Vector3 direction = new Vector3(0f, 0f, -_CurrentZoom);
        Quaternion rotation = Quaternion.Euler(_Pitch, _Yaw, 0f);
        Vector3 target_position = Target.position + rotation * direction;

        // Check water surface.
        if (WaterSurface.Instance != null)
        {
            float wave_height = WaterSurface.Instance.GetHeightAt(target_position);
            float min_y = wave_height + MinimalHeightAboveWater;
            if (target_position.y < min_y)
                target_position.y = min_y;
        }

        // Apply the position and calculate rotation.
        transform.position = target_position;
        transform.LookAt(Target.position);
    }
}