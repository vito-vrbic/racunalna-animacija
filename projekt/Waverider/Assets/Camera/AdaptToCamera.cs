using UnityEngine;

public class AdaptToCamera : MonoBehaviour
{
    #region REGION: Public Properties
    [Header("Offset")]
    public float PositionOffset = 100f;
    public float RotationOffset = 45f;
    #endregion

    #region REGION: Private Fields
    private Transform _Camera;
    #endregion

    private void Start()
    {
        _Camera = Camera.main.transform;
    }

    void Update()
    {
        if (_Camera == null)
            return;

        // Remove height from the forward vector.
        Vector3 forward = _Camera.forward;
        forward.y = 0f;

        // Don't do anything if the forward vector is up or down.
        if (forward.sqrMagnitude < 0.0001f)
            return;

        // Normalize the vector on the XZ plane.
        forward.Normalize();

        SetPosition(forward);
        SetRotation(forward);
    }

    void SetPosition(Vector3 forward)
    {
        // Find a fixed distance from the camera (in the direction of the heightless forward vector).
        Vector3 target = _Camera.position + forward * (PositionOffset);
        target.y = transform.position.y;

        // Set position.
        transform.position = target;
    }

    void SetRotation(Vector3 forward)
    {
        // Find a rotation relative to camera forward.
        Quaternion target = Quaternion.LookRotation(forward);
        target *= Quaternion.Euler(0f, RotationOffset, 0f);

        // Set rotation.
        transform.rotation = target;
    }
}