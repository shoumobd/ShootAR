using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class meshing : MonoBehaviour
{
    public static Mesh viewedModel;

    public GameObject game;
    //Renderer renderer;

    // Start is called before the first frame update
    void Start()
    {
        MeshFilter viewedModelFilter = (MeshFilter)game.GetComponent("MeshFilter");
        viewedModel = viewedModelFilter.mesh;
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
