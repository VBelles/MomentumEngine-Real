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
        //string system.colorOverLifetime.color.colorMin.r
    }

    void Export(ParticleSystem system)
    {
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
                startColor = system.main.startColor.color.r + " " + system.main.startColor.color.g + " " + system.main.startColor.color.b + " " + system.main.startColor.color.a,
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
            shape = system.shape.enabled ? new Shape {
                shapeType = system.shape.shapeType.ToString(),
                radiusMode = system.shape.radiusMode.ToString(),
                randomDirectionAmount = system.shape.randomDirectionAmount,
                sphericalDirectionAmount = system.shape.sphericalDirectionAmount,
                radius = system.shape.radius,
                radiusSpeed = system.shape.radiusSpeed.constant,
                angle = system.shape.angle,
                alignToDirection = system.shape.alignToDirection
            } : null,
            colorOverLifetime = system.colorOverLifetime.enabled ? new ColorOverLifetime {
                color = new string[]{
                system.colorOverLifetime.color.Evaluate(0).r        + " " + system.colorOverLifetime.color.Evaluate(0).g        + " " + system.colorOverLifetime.color.Evaluate(0).b        + " " + system.colorOverLifetime.color.Evaluate(0).a,
                system.colorOverLifetime.color.Evaluate(0.125f).r   + " " + system.colorOverLifetime.color.Evaluate(0.125f).g   + " " + system.colorOverLifetime.color.Evaluate(0.125f).b   + " " + system.colorOverLifetime.color.Evaluate(0.125f).a,
                system.colorOverLifetime.color.Evaluate(0.25f).r    + " " + system.colorOverLifetime.color.Evaluate(0.25f).g    + " " + system.colorOverLifetime.color.Evaluate(0.25f).b    + " " + system.colorOverLifetime.color.Evaluate(0.25f).a,
                system.colorOverLifetime.color.Evaluate(0.375f).r   + " " + system.colorOverLifetime.color.Evaluate(0.375f).g   + " " + system.colorOverLifetime.color.Evaluate(0.375f).b   + " " + system.colorOverLifetime.color.Evaluate(0.375f).a,
                system.colorOverLifetime.color.Evaluate(0.5f).r     + " " + system.colorOverLifetime.color.Evaluate(0.5f).g     + " " + system.colorOverLifetime.color.Evaluate(0.5f).b     + " " + system.colorOverLifetime.color.Evaluate(0.5f).a,
                system.colorOverLifetime.color.Evaluate(0.625f).r   + " " + system.colorOverLifetime.color.Evaluate(0.625f).g   + " " + system.colorOverLifetime.color.Evaluate(0.625f).b   + " " + system.colorOverLifetime.color.Evaluate(0.625f).a,
                system.colorOverLifetime.color.Evaluate(0.75f).r    + " " + system.colorOverLifetime.color.Evaluate(0.75f).g    + " " + system.colorOverLifetime.color.Evaluate(0.75f).b    + " " + system.colorOverLifetime.color.Evaluate(0.75f).a,
                system.colorOverLifetime.color.Evaluate(0.875f).r   + " " + system.colorOverLifetime.color.Evaluate(0.875f).g   + " " + system.colorOverLifetime.color.Evaluate(0.875f).b   + " " + system.colorOverLifetime.color.Evaluate(0.875f).a,
                system.colorOverLifetime.color.Evaluate(1).r        + " " + system.colorOverLifetime.color.Evaluate(1).g        + " " + system.colorOverLifetime.color.Evaluate(1).b        + " " + system.colorOverLifetime.color.Evaluate(1).a,
                
                }   
            }: null//,
            //sizeOverLifetime = system.sizeOverLifetime.enabled ? new SizeOverLifetime {

            //}: null,
            //renderer =  new Renderer {

            //}
        };
        string dataAsJson = JsonUtility.ToJson(p, true);
        string filePath = Application.dataPath + "/" + system.name + ".json";
        File.WriteAllText(filePath, dataAsJson);
    }
}
