using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class hbar : MonoBehaviour
{
    private GameObject bar;
    // Start is called before the first frame update
    void Start()
    {
        bar = GameObject.Find("barh");

    }

    // Update is called once per frame
    public void setsize(int barsize)
    {
        if(barsize==1)
        bar.transform.localScale = new Vector3(.15f, .5f);
        if (barsize == 2)
            bar.transform.localScale = new Vector3(.1f, .5f);
        if (barsize == 3)
            bar.transform.localScale = new Vector3(0f, .5f);
    }
}
