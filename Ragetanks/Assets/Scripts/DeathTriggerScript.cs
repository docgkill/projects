using UnityEngine;
using System.Collections;

public class DeathTriggerScript : MonoBehaviour {

	void OnTriggerEnter2D( Collider2D collidedObject )  {
//		Debug.Log ("DeathTriggerScript docgkill hit");
		collidedObject.SendMessage("hitDeathTrigger");
	}
}