using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[Serializable]
public class ParticleSystemStruct
{
    public Main main;
    public Emission emission;
    public Shape shape;
    public ColorOverLifetime colorOverLifetime;
    public SizeOverLifetime sizeOverLifetime;
    public Renderer renderer;
}

[Serializable]
public class Main
{
    public float duration;
    public bool loop;
    public bool prewarm;
    public float startDelay;
    public float startLifetime;
    public float startSpeed;
    public bool startSize3D;
    public float startSize;
    public bool startRotation3D;
    public float startRotation;
    public float randomizeRotationDirection;
    public string startColor;
    public float gravityModifier;
    public float simulationSpeed;
    public bool playOnAwake;
    public int maxParticles;
}


[Serializable]
public class Emission
{
    public float rateOverTime;
    public float rateOverDistance;
}

[Serializable]
public class Shape
{
    public string shapeType;
    public string radiusMode;
    public float randomDirectionAmount;
    public float sphericalDirectionAmount;
    public float radius;
    public float radiusSpeed;
    public float angle;
    public bool alignToDirection;
}

[Serializable]
public class ColorOverLifetime
{
    public string[] color;
    //public string colorMax;
}

[Serializable]
public class SizeOverLifetime
{

}

[Serializable]
public class Renderer
{

}