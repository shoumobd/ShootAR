using UnityEngine;
using System.Collections;

public class abcd : MonoBehaviour
{
    public WebCamTexture mCamera = null;
    public GameObject plane;

    // Use this for initialization
    void Start()
    {
        Debug.Log("Script has been started");
        plane = GameObject.FindWithTag("Player");
 
        mCamera = new WebCamTexture();
        GetComponent<Renderer>().material.mainTexture = mCamera;
        mCamera.Play();

    }

    // Update is called once per frame
    void Update()
    {

    }
}