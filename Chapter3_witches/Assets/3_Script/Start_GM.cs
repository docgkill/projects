using UnityEngine;
using System.Collections;

public class Start_GM : MonoBehaviour {

	public AudioClip _play;
	public AudioClip _dammEffect;

	public void startGame()  {
		Application.LoadLevel ("1_play");
		audio.clip = _play;
		audio.Play ();
	}


	void Awake()  {
		DontDestroyOnLoad (gameObject);
	}
}
