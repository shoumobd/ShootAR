using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class uicomponent : MonoBehaviour
{
    public Text score_text;
//public Text gameover_text;
    public int score=0;
    // Start is called before the first frame update
    
    public void scoreupdate()
    {
        //Debug.Log("score = " + score);
        score += 10;
        score_text.text = "SCORE: " + score;
        
    }
    
    public int scoreget()
    {
        return score;
    }

}
