using System.Collections;
using System.Collections.Generic;
using UnityEngine;

// Class which holds all the data on a single Gerstner wave.
public class GerstnerWave
{
    public float Height = 1f;
    public float Frequency = 0.05f;
    public float Speed = 4;
    public Vector3 Direction = new Vector3(1, 0, 1);
    public float Peak = 0.5f;
}
