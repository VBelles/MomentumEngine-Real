using System.Collections;
using System.Collections.Generic;
using System.IO;
using UnityEngine;

public class ParticleExporter : MonoBehaviour {
    public ParticleSystem system;

	// Use this for initialization
	void Start () {

        Export(system);
        //system.main.duration;
        //bool system.main.loop;
        //bool system.main.prewarm;
        //float dfjndf = system.main.startDelay.constant;
        //float system.main.startLifetime.constant;
        //float system.main.startSpeed.constant;
        //bool system.main.startSize3D;
        //float system.main.startSize;
        //bool system.main.startRotation3D;
        //float system.main.startRotation;
        //float system.main.randomizeRotationDirection;
        //string color = system.main.startColor.color;
        //float system.main.gravityModifier;
        //float system.main.simulationSpeed;
        //bool system.main.playOnAwake;
        //int system.main.maxParticles;

    }
	
    void Export(ParticleSystem system)
    {
        ParticleSystemStruct p = new ParticleSystemStruct
        {
            main = new Main {
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
            }//,
            //emission = system.emission.enabled ? new Emission {
                 
            //} : null,
            //shape = system.shape.enabled ? new Shape {

            //} : null,
            //colorOverLifetime = system.colorOverLifetime.enabled ? new ColorOverLifetime {

            //}: null,
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
