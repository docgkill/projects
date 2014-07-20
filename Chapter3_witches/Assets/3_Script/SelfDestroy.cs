using UnityEngine;
using System.Collections;

public class SelfDestroy : MonoBehaviour {

	private float timerForDel = 0f;
	public float timerForDelLimit;

	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
		timerForDel += Time.deltaTime;
		if (timerForDel > timerForDelLimit) {
			Destroy(gameObject);
		}
	
	}
}
