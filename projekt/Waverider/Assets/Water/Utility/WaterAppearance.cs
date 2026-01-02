using System.Collections;
using System.Collections.Generic;
using UnityEngine;

// Class which holds all the information on the water appearance.
public class WaterAppearance
{
    public float Depth = 0.4f;
    public Color DeepColor = Color.black;
    public Color ShallowColor = Color.blue;
    public float RefractionSpeed = 0.05f;
    public float RefractionScale = 1;
    public float RefractionStrength = 2;
    public float FoamAmount = 1.3f;
    public float FoamCutoff = 1.1f;
    public float FoamSpeed = 1;
    public float FoamScale = 132.5f;
    public Color FoamColor = Color.gray;
}
