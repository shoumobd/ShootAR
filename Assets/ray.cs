using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class ray : MonoBehaviour

{
    public GameObject e1,e2,e3,e4,e5,e6,e7,ab,ab2,go;
    public Text go_text,timer_text,wave_text,ammo_text,score_text;
    //public Text wave_text;
    public int cnt1,cnt2,cnt3,cnt4,cnt5,cnt6,cnt7,w,f1,am;
    public int bal = 0;
    public float t1,t2,time;
    public Vector3 e1p, e2p,e3p,e4p,e5p,e6p,e7p,abp,ab2p;
    private uicomponent _uicomp;
    private hbar _hbar;

    private AudioSource _audioSource;
    private AudioSource gameover;
    public bool bonus = false;


    //new Vector3 pos = (3, 3, 3);
    // Start is called before the first frame update

    public int score = 0;
   

    void Start()
    {
        Debug.Log("start button");
        e1.SetActive(true);
        e2.SetActive(true);
        e3.SetActive(true);
        e4.SetActive(true);
        e5.SetActive(true);
        e6.SetActive(false);
        e7.SetActive(false);
        ab.SetActive(false); //ammobox
        ab2.SetActive(false); //ammobox boss
        go.SetActive(false); //gameover
        
        e1p = e1.transform.localPosition;
        e2p = e2.transform.localPosition;
        e3p = e3.transform.localPosition;
        e4p = e4.transform.localPosition;
        e5p = e5.transform.localPosition;
        e6p = e6.transform.localPosition;
        e7p = e7.transform.localPosition;
        abp = ab.transform.localPosition;
        ab2p = ab2.transform.localPosition;
        //cnt = 0;
        cnt1 = 0;
        cnt2 = 0;
        cnt3 = 0;
        cnt4 = 0;
        cnt5 = 0;
        cnt6 = 0;
        cnt7 = 0;
        w = 1;
        f1 = 0;
        am = 7;
        time = 15.0f;
        t2 = 2.0f;
        //t1 = 10.0f;
        _uicomp = GameObject.Find("Canvas").GetComponent<uicomponent>();
        _hbar = GameObject.Find("barh").GetComponent<hbar>();
        _audioSource = GetComponent<AudioSource>();
        _hbar.gameObject.SetActive(false);
        //_audio[0] = _audioSource;
        // _audio[1] = gameover;
    }

    //public GameObject barb;
    //public GameObject dupbarb;
    public bool candup = false;
    // Update is called once per frame
    // IEnumerator dupl()
    //{
    //  Instantiate(dupbarb,new Vector3(barb.transform.position.x+1,barb.transform.position.y+1,barb.transform.position.z), Quaternion.identity);
    //yield return new WaitForSeconds(200f);
    //}

    public bool wcomp()
    {
        if(w%2 != 0)
        {
            if (cnt1 + cnt2 + cnt3 + cnt4 + cnt5 >= 5)
            {
                return true;
            }
            return false;
        }
        else if (w % 2 == 0 && w % 4 != 0)
        {
            if (cnt1 + cnt2 + cnt3 + cnt4 + cnt5 >= 5 && cnt6 >= 3)
            {
                return true;
            }
            return false;
        }
        else
        {
            if (cnt1 + cnt2 + cnt3 + cnt4 + cnt5 >= 5 && cnt7 >= 3)
            {
                return true;
            }
            return false;
        }
    }

    public void updatescore()
    {
        //Debug.Log("score = " + score);
        score += 10;
        score_text.text = "SCORE: " + score;

    }

    public int getscore()
    {
        return score;
    }

    void Update()
    {
       // Debug.Log("MARA UP ");
        wave_text.text = "WAVE : " + w;
        ammo_text.text = "AMMO REMAINING : " + am;
        if(time>0.0f) time -= Time.deltaTime;
        timer_text.text = "TIME REMAINING : " + (int) time + " SEC ";
        //Debug.Log("time = " + time);
        if (time <= 0.0f)
        {
            go.SetActive(true);
           // gameover.Play();
            go_text.text = "GAME OVER! \n SCORE : " +getscore();
            //gameObject.SetActive(false);
            if (t2 > 0.0f)
            {
                //Debug.Log("MARA KAO "+t2);
                t2 -= Time.deltaTime;
                if (t2 <= 0.0f)
                    //Application.Quit();
                    UnityEditor.EditorApplication.isPlaying = false;
            }
            //UnityEditor.EditorApplication.isPlaying = false;
        }
        if (t1 > 0.0f)
        {
            t1 -= Time.deltaTime;
            if (t1 <= 0.0f) go.SetActive(false);
        }
        //Debug.Log("t1 = " + t1);
        //if (cnt1>0 && cnt2>0 && cnt3>0 && cnt4 > 0 && cnt5 > 0)
        //if (cnt1 + cnt2 + cnt3 + cnt4 + cnt5 >= 5)
        if(wcomp())
        {
         // e1.transform.position = new Vector3(1, 1, 1);
            //e1.transform.position = new Vector3(Random.Range(-1f, 3f), Random.Range(-.3f, .4f), Random.Range(8.5f, 9.5f));
            //e2.transform.position = new Vector3(Random.Range(-2f, 1f), Random.Range(-.2f, .3f), Random.Range(8f, 9f));
            w++;
            e1.SetActive(true);
            e2.SetActive(true);
            e3.SetActive(true);
            e4.SetActive(true);
            e5.SetActive(true);
            if (w % 2 > 0)
            {
                ab.transform.position = new Vector3(Random.Range(-1f, 3f), Random.Range(-.3f, .4f), Random.Range(8.5f, 9.5f));
                ab.SetActive(true);
            }
            else
            {
                ab2.transform.position = new Vector3(Random.Range(-1f, 3f), Random.Range(-.3f, .4f), Random.Range(8.5f, 9.5f));
                ab2.SetActive(true);
            }
            if (w % 2 == 0 && w % 4 != 0)
            {
                e6.SetActive(true);
                _hbar.gameObject.SetActive(true);
                //am += 8;
            }
            else if (w % 4 == 0)
            {
                e7.SetActive(true);
                _hbar.gameObject.SetActive(true);
                //am += 8;
            }
            else
            {
                //am += 6;
            }
            cnt1 = 0;
            cnt2 = 0;
            cnt3 = 0;
            cnt4 = 0;
            cnt5 = 0;
            cnt6 = 0;
            cnt7 = 0;
            bal = (int)time;
            time = 15.0f;
            go.SetActive(true);
            t1 = 2.0f;
            
            go_text.text = "WAVE COMPLETE! \nTIME BONUS: "+bal;
            
            score += (bal * 5);
            //score += (bal);
            bonus = false;
            //Debug.Log("bonus false");
            score_text.text = "SCORE: " + score;
            
            /*else
            {
                t2 = 2.0f;
                go_text.text = "Game Over! \nScore: -1";// + score;
                //Application.Quit();
                //UnityEditor.EditorApplication.isPlaying = false;
            }*/
        }
        if (Input.GetMouseButton(0)|| Input.GetKey("space"))
        {
            _audioSource.Play();
            if (f1>0 || am<=0 || time<=0.0f)
            {
                f1 = 1;
            }
            else
            {

                f1 = 1;
                am--;
                /*Debug.Log("cnt1 = " + cnt1);
                Debug.Log("cnt2 = " + cnt2);
                Debug.Log("cnt3 = " + cnt3);
                Debug.Log("cnt4 = " + cnt4);
                Debug.Log("cnt5 = " + cnt5);*/
                //Debug.Log("input button");
                Ray rayOrigin = Camera.main.ScreenPointToRay(new Vector3(Screen.width / 2, Screen.height / 2, 0));
                RaycastHit hitInfo;
                //Debug.Log("ray casted");
                Debug.DrawRay(transform.TransformDirection(Vector3.forward), Vector3.forward, Color.red, 5f);
                if (Physics.Raycast(rayOrigin, out hitInfo, Mathf.Infinity))
                {
                    //Debug.Log("HIT hoise: " + hitInfo.transform.name);
                    Debug.DrawRay(transform.TransformDirection(Vector3.forward), hitInfo.point, Color.red, 5f);
                    //StartCoroutine("dupl");
                    candup = true;
                    //Debug.Log("candup is true");

                    //Destroy(barb);
                    if (hitInfo.transform.name == "Meshtint Barbarian Green")
                    {
                        //Debug.Log("In if green");
                        //if (e1.active)
                        {
                            //updatescore();
                            score += 10;
                            score_text.text = "SCORE: " + score;
                        }
                        e1.SetActive(false);
                        //e1.transform.position= new Vector3(0, 0, 0);
                        //e1.transform.position = e1p;
                        //e2.transform.position = e2p;
                        cnt1 = 1;

                        //Debug.Log("cnt1 = "+cnt1);
                    }
                    if (hitInfo.transform.name == "Meshtint Barbarian Gold")
                    {
                        //if (e2.active)
                        {
                            //updatescore();
                            score += 10;
                            score_text.text = "SCORE: " + score;
                        }
                        e2.SetActive(false);
                        cnt2 = 1;
                        //Debug.Log("cnt2 = "+cnt2);
                    }
                    if (hitInfo.transform.name == "Meshtint Barbarian Gold 2")
                    {
                        //if (e3.active)
                        {
                            //updatescore();
                            score += 10;
                            score_text.text = "SCORE: " + score;
                        }
                        e3.SetActive(false);
                        cnt3 = 1;
                        //Debug.Log("cnt3 = " + cnt3);
                    }
                    if (hitInfo.transform.name == "Meshtint Barbarian Red")
                    {
                        //if (e4.active)
                        {
                            //updatescore();
                            score += 10;
                            score_text.text = "SCORE: " + score;
                        }
                        e4.SetActive(false);
                        cnt4 = 1;
                        //Debug.Log("cnt4 = " + cnt4);
                    }
                    if (hitInfo.transform.name == "Meshtint Barbarian Red 2")
                    {
                        //if (e5.active)
                        {
                            //updatescore();
                            score += 10;
                            score_text.text = "SCORE: " + score;
                        }
                        e5.SetActive(false);
                        cnt5 = 1;
                        //Debug.Log("cnt5 = " + cnt5);
                    }
                    if (hitInfo.transform.name == "Boss 1")
                    {
                        //_hbar.gameObject.SetActive(true);
                        /*if (e6.active)
                        {
                            //updatescore();
                        }*/
                        cnt6++;
                        e6.transform.position = new Vector3(Random.Range(-3f, 5f), e6.transform.position.y, Random.Range(3f, 8f));
                        
                        _hbar.setsize(cnt6);                        
                        if (cnt6 >= 3)
                        {
                            score += 50;
                            score_text.text = "SCORE: " + score;
                            e6.SetActive(false);
                           // _hbar.gameObject.SetActive(false);
                        }
                        //cnt6 = 1;
                        //Debug.Log("cnt6 = " + cnt6);
                    }
                    if (hitInfo.transform.name == "Boss 2")
                    {
                        cnt7++;
                        e7.transform.position = new Vector3(Random.Range(-3f, 4f), e7.transform.position.y, Random.Range(3f, 6f));
                        _hbar.setsize(cnt7);
                        if (cnt7 >= 3)
                        {
                            score += 50;
                            score_text.text = "SCORE: " + score;
                            e7.SetActive(false);
                        }
                        //cnt7 = 1;
                        //Debug.Log("cnt7 = " + cnt7);
                    }

                    if (hitInfo.transform.name == "AmmoBox")
                    {
                        am += 7;
                        ab.SetActive(false);
                    }
                    if (hitInfo.transform.name == "AmmoBox2")
                    {
                        am += 10;
                        ab2.SetActive(false);
                    }
                }
                //else
                //Debug.Log("no ray");

            }
        }

        else
        {
            f1 = 0;
        }
    
    }
}
