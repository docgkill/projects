using UnityEngine;
using System.Collections;

public class BG_Move : MonoBehaviour {
	public float _speed = 5;
	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
		transform.Translate (_speed*Time.deltaTime, 0, 0);

		if(transform.localPosition.x<-3200)  {
			transform.localPosition = new Vector3(-640, 0, 0);
		}
	}
}
