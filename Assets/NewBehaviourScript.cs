using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ARCamera : MonoBehaviour
{

    static WebCamTexture backCam;

    void Start()
    {
        WebCamDevice[] devices = WebCamTexture.devices;
        WebCamTexture webCamTexture = new WebCamTexture(devices[1].name);
        GetComponent<Renderer>().material.mainTexture = webCamTexture;
        webCamTexture.filterMode = FilterMode.Trilinear;
        webCamTexture.Play();

    }

    void Update()
    {

    }
}