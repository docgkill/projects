using UnityEngine;
using System.Collections;

public class SetQ : MonoBehaviour {

	public int rederQueue;

	// Use this for initialization
	void Awake () {
		UIPanel parentPanel = GetComponentsInParent<UIPanel> ()[0];
//		Debug.Log ("reqqqqqqqq    " + parentPanel.renderer.material.renderQueue);
	//	GameObject ob = transform.parent;
	//	Debug.Log ("qqqqqqqq " + parentPanel.;

		//Sprite s = GetComponentInParent<Sprite> (); 
	//	s.Mater
	//	rederQueue = transform.parent.renderer.material.renderQueue;
//		rederQueue = 3002;
	//	parentPanel.transform.renderer.material.renderQueue
		Debug.Log ("reder " + parentPanel.renderQueue);
		//parentPanel.renderQueue
		//	UIPanel.Rendereq
		transform.renderer.material.renderQueue = 3000;

	}
	
	// Update is called once per frame
	void Update () {
//		rederQueue = transform.parent.renderer.material.renderQueue;
	//	rederQueue = 3002;
//		transform.renderer.material.renderQueue = rederQueue;
	}


	public void pauseuisprite()  {
		Debug.Log ("pa");
		GetComponent<UISprite> ().enabled = false;;
	}
}
