using System.Collections;
using System.Collections.Generic;
using System.IO;
using UnityEngine;

public class ParticleExporter : MonoBehaviour
{
    public ParticleSystem system;

    // Use this for initialization
    void Start()
    {
        Export(system);
        //system.
    }

    void Export(ParticleSystem system)
    {
        ParticleSystemRenderer systemRenderer = system.GetComponent<ParticleSystemRenderer>();
        Color systemStartColor = system.main.startColor.color;
        ParticleSystem.MinMaxGradient colorGradient = new ParticleSystem.MinMaxGradient();
        if (system.colorOverLifetime.enabled)
        {
            colorGradient = system.colorOverLifetime.color;
        }

        ParticleSystemStruct p = new ParticleSystemStruct
        {
            main = new Main
            {
                duration = system.main.duration,
                loop = system.main.loop,
                prewarm = system.main.prewarm,
                startDelay = system.main.startDelay.constant,
                startLifetime = system.main.startLifetime.constant,
                startSpeed = system.main.startSpeed.constant,
                startSize3D = system.main.startSize3D,
                startSize = system.main.startSize.constant,
                startRotation3D = system.main.startRotation3D,
                startRotation = system.main.startRotation.constant,
                randomizeRotationDirection = system.main.randomizeRotationDirection,
                startColor = systemStartColor.r + " " + systemStartColor.g + " " + systemStartColor.b + " " + systemStartColor.a,
                gravityModifier = system.main.gravityModifier.constant,
                simulationSpeed = system.main.simulationSpeed,
                playOnAwake = system.main.playOnAwake,
                maxParticles = system.main.maxParticles
            },
            emission = system.emission.enabled ? new Emission
            {
                rateOverTime = system.emission.rateOverTime.constant,
                rateOverDistance = system.emission.rateOverDistance.constant
            } : null,
            shape = system.shape.enabled ? new Shape
            {
                shapeType = system.shape.shapeType.ToString(),
                radiusMode = system.shape.radiusMode.ToString(),
                randomDirectionAmount = system.shape.randomDirectionAmount,
                sphericalDirectionAmount = system.shape.sphericalDirectionAmount,
                radius = system.shape.radius,
                radiusSpeed = system.shape.radiusSpeed.constant,
                angle = system.shape.angle,
                alignToDirection = system.shape.alignToDirection
            } : null,
            colorOverLifetime = system.colorOverLifetime.enabled ? new ColorOverLifetime
            {
                color = new string[]{
                colorGradient.Evaluate(0).r        + " " + colorGradient.Evaluate(0).g        + " " + colorGradient.Evaluate(0).b        + " " + colorGradient.Evaluate(0).a,
                colorGradient.Evaluate(0.125f).r   + " " + colorGradient.Evaluate(0.125f).g   + " " + colorGradient.Evaluate(0.125f).b   + " " + colorGradient.Evaluate(0.125f).a,
                colorGradient.Evaluate(0.25f).r    + " " + colorGradient.Evaluate(0.25f).g    + " " + colorGradient.Evaluate(0.25f).b    + " " + colorGradient.Evaluate(0.25f).a,
                colorGradient.Evaluate(0.375f).r   + " " + colorGradient.Evaluate(0.375f).g   + " " + colorGradient.Evaluate(0.375f).b   + " " + colorGradient.Evaluate(0.375f).a,
                colorGradient.Evaluate(0.5f).r     + " " + colorGradient.Evaluate(0.5f).g     + " " + colorGradient.Evaluate(0.5f).b     + " " + colorGradient.Evaluate(0.5f).a,
                colorGradient.Evaluate(0.625f).r   + " " + colorGradient.Evaluate(0.625f).g   + " " + colorGradient.Evaluate(0.625f).b   + " " + colorGradient.Evaluate(0.625f).a,
                colorGradient.Evaluate(0.75f).r    + " " + colorGradient.Evaluate(0.75f).g    + " " + colorGradient.Evaluate(0.75f).b    + " " + colorGradient.Evaluate(0.75f).a,
                colorGradient.Evaluate(0.875f).r   + " " + colorGradient.Evaluate(0.875f).g   + " " + colorGradient.Evaluate(0.875f).b   + " " + colorGradient.Evaluate(0.875f).a,
                colorGradient.Evaluate(1).r        + " " + colorGradient.Evaluate(1).g        + " " + colorGradient.Evaluate(1).b        + " " + colorGradient.Evaluate(1).a
                }
            } : null,
            sizeOverLifetime = system.sizeOverLifetime.enabled ? new SizeOverLifetime
            {
                size = new float[]{
                    system.sizeOverLifetime.size.Evaluate(0),
                    system.sizeOverLifetime.size.Evaluate(0.125f),
                    system.sizeOverLifetime.size.Evaluate(0.25f),
                    system.sizeOverLifetime.size.Evaluate(0.375f),
                    system.sizeOverLifetime.size.Evaluate(0.5f),
                    system.sizeOverLifetime.size.Evaluate(0.625f),
                    system.sizeOverLifetime.size.Evaluate(0.75f),
                    system.sizeOverLifetime.size.Evaluate(0.875f),
                    system.sizeOverLifetime.size.Evaluate(1)
                }
            } : null,
            renderer = systemRenderer.enabled ? new Renderer
            {
                renderMode = systemRenderer.renderMode.ToString(),
                shadowCastingMode = systemRenderer.shadowCastingMode.ToString(),
                normalDirection = systemRenderer.normalDirection,
                minParticleSize = systemRenderer.minParticleSize,
                maxParticleSize = systemRenderer.maxParticleSize,
                receiveShadows = systemRenderer.receiveShadows
            } : null
        };
        string dataAsJson = JsonUtility.ToJson(p, true);
        string filePath = Application.dataPath + "/" + system.name + ".json";
        File.WriteAllText(filePath, dataAsJson);
    }
}
