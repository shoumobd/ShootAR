using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class duplicate : MonoBehaviour
{
    public GameObject game;
    // Start is called before the first frame update
    void Start()
    {

    }
    IEnumerator dup()
    {
        Instantiate(game,new Vector3(Random.Range(5,10),1,10), Quaternion.identity);
        yield return new WaitForSeconds(1f);
        Destroy(game);
        
    }
    // Update is called once per frame
    void Update()
    {
        if (gameObject.GetComponent<ray>().candup == true)
        {
            //Debug.Log("duplicate");
            StartCoroutine("dup");
        }

    }
}
