using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class spawn : MonoBehaviour
{
    public GameObject enemyship;
    // Start is called before the first frame update
    void Start()
    {
        StartCoroutine(enemyspawn());
    }

    IEnumerator enemyspawn()
    {
        while (true)
        {
            Instantiate(enemyship, new Vector3(Random.Range(-11f, 11f), 6, 0), Quaternion.identity);
            yield return new WaitForSeconds(.5f);
            Destroy(enemyship, 1.0f);
        }
    }
}
