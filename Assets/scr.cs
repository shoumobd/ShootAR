using UnityEngine;
using System.Collections;

public class scr : MonoBehaviour
{

    /*public float horizontalSpeed;
    public float verticalSpeed;
    public float amplitude;
    //public GameObject game;

    private Vector3 tempPosition;

    void Start()
    {
        tempPosition = gameObject.transform.position;
        
    }

    void FixedUpdate()
    {

        tempPosition.x += horizontalSpeed;
        tempPosition.y += verticalSpeed;
        tempPosition.z += amplitude;

        gameObject.transform.Rotate(Vector3.down * 5.0f * Time.deltaTime);
        tempPosition.y = Mathf.Sin(Time.realtimeSinceStartup * verticalSpeed); //* amplitude;
        tempPosition.x = Mathf.Sin(Time.realtimeSinceStartup * horizontalSpeed); //* amplitude;
        tempPosition.z = Mathf.Sin(Time.realtimeSinceStartup * amplitude); //* amplitude;

        transform.position = tempPosition;


        //int x <.6>
         //qwqe z<.8
            //qdd y<>
    }*/




    Vector3 op;
    public float tv=.7f;

    void Start()
    {
        //tv = Random.Range(.4f, 1.0f);
        op = transform.localPosition;
    }

    void Update()
    {


        if (Mathf.Abs(gameObject.transform.position.x-op.x)>=.5)
        {
            tv *= (-1.0f);
        }
        transform.Translate(new Vector3(tv, 0, 0) * Time.deltaTime);
        /*else
        {
            transform.Translate(new Vector3(-0.5f, 0, 0) * Time.deltaTime);
        }*/
    }
}