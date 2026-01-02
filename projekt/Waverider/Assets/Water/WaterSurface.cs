using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class WaterSurface : MonoBehaviour
{
    public static WaterSurface Instance;

    #region REGION: Private Material Fields
    GerstnerWave[] _waves = new GerstnerWave[3];
    WaterAppearance _appearance = new WaterAppearance();
    #endregion

    Material _material;

    void Start()
    {
        // Initializing instance.
        if (Instance) Destroy(this);
        Instance = this;

        // Getting material instance.
        Renderer renderer = GetComponent<Renderer>();
        _material = renderer.material;

        // Making the mesh bounds bigger so it stops disappearing at certain camera angles.
        MeshFilter mf = GetComponent<MeshFilter>();
        Bounds b = mf.mesh.bounds;
        b.Expand(new Vector3(0, 100, 0));
        mf.mesh.bounds = b;

        // Reading appearance parameters from the material.
        _appearance.Depth = _material.GetFloat("_Depth");
        _appearance.DeepColor = _material.GetColor("_DeepColor");
        _appearance.ShallowColor = _material.GetColor("_ShallowColor");

        _appearance.RefractionSpeed = _material.GetFloat("_RefractionSpeed");
        _appearance.RefractionScale = _material.GetFloat("_RefractionScale");
        _appearance.RefractionStrength = _material.GetFloat("_RefractionStrength");

        _appearance.FoamAmount = _material.GetFloat("_FoamAmount");
        _appearance.FoamCutoff = _material.GetFloat("_FoamCutoff");
        _appearance.FoamSpeed = _material.GetFloat("_FoamSpeed");
        _appearance.FoamScale = _material.GetFloat("_FoamScale");
        _appearance.FoamColor = _material.GetColor("_FoamColor");

        // Reading wave parameters from the material.
        for (int i = 1; i <= _waves.Length; i++)
        {
            GerstnerWave w = new GerstnerWave();

            // Pull wave parameters directly from the material
            w.Height = _material.GetFloat($"_{i}_WaveHeight");
            w.Frequency = _material.GetFloat($"_{i}_Frequency");
            w.Speed = _material.GetFloat($"_{i}_WaveSpeed");
            w.Direction = _material.GetVector($"_{i}_WaveDirection");
            w.Peak = _material.GetFloat($"_{i}_Peak");

            // Store in the waves array
            _waves[i - 1] = w;
        }
    }


    public float GetHeightAt(Vector3 world_pos)
    {
        // Converting world position to XZ plane for 2D sampling.
        Vector2 target_xz = new Vector2(world_pos.x, world_pos.z);
        Vector2 sample_xz = target_xz;

        float time = Time.time;
        const int iterations = 4;

        // Iteratively adjusting sample point for horizontal Gerstner displacement.
        for (int it = 0; it < iterations; it++)
        {
            Vector2 total_horizontal = Vector2.zero;

            for (int i = 0; i < _waves.Length; i++)
            {
                GerstnerWave w = _waves[i];

                Vector2 dir = new Vector2(w.Direction.x, w.Direction.z).normalized;

                float k = w.Frequency;
                float omega = k * w.Speed;

                float phase = k * Vector2.Dot(dir, sample_xz) + omega * time;

                float cos_p = Mathf.Cos(phase);

                total_horizontal += (w.Peak * w.Height * dir * cos_p) / _waves.Length;
            }

            // Pulling sample points back toward original surface position.
            sample_xz = target_xz - total_horizontal;
        }

        // Computing final height at the corrected position.
        float height = 0f;

        for (int i = 0; i < _waves.Length; i++)
        {
            GerstnerWave w = _waves[i];

            Vector2 dir = new Vector2(w.Direction.x, w.Direction.z).normalized;

            float k = w.Frequency;
            float omega = k * w.Speed;

            float phase = k * Vector2.Dot(dir, sample_xz) + omega * time;

            height += w.Height * Mathf.Sin(phase);
        }

        return height;
    }
}
