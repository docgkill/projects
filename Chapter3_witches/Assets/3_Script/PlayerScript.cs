using UnityEngine;
using System.Collections;

public class PlayerScript : MonoBehaviour {

	public int _hp = 100;
	public float _speed = 5.0f;
	private float _halfHeight;

	public Animator _anim;

	public GameObject _damageEffect;

	public UISprite _gaugeBar;
	public UILabel _score;

	public UIPanel _uiPanel;

	public Start_GM _Start_GM;



	// Use this for initialization
	void Start () {
				Debug.Log ("=========start");
				_halfHeight = Screen.height / 2;

				_Start_GM = GameObject.Find ("Start_GM").GetComponent<Start_GM> ();

	}
	
	// Update is called once per frame
	void Update () {
		if (Input.touchCount > 0) {
			float deltaPosition = Input.GetTouch(0).position.y - _halfHeight;
			float posY = deltaPosition - transform.position.y;
			Debug.Log("posY "+posY);
			transform.Translate(0f, _speed*Time.deltaTime*posY*0.01f, 0f);

			transform.localPosition = new Vector3(transform.position.x, 
			                                      Mathf.Clamp(transform.localPosition.y, -270f, 250f), 
			                                      transform.localPosition.z);
		}

		_score.text = (Time.timeSinceLevelLoad * 100).ToString ("N0"); 


	
	}


//	void OnTriggerEnter(Collider other)  {
	void OnTriggerEnter2D( Collider2D collidedObject )  {
		Debug.Log ("========= collier");
		_hp--;
		_gaugeBar.fillAmount = _hp * 0.01f;

		if (_anim != null) {
			_anim.SetBool("damageChk", true);

			var damage = Instantiate(_damageEffect, transform.localPosition, Quaternion.identity) as GameObject;
		//	var damage = Instantiate(_damageEffect, new Vector3(100,100,0), Quaternion.identity) as GameObject;
			damage.transform.parent = transform;
			damage.transform.localPosition = new Vector3(-1, 0,0);;
			damage.transform.localScale = new Vector3(1,1,1);
		}


		if (_hp <= 0) {
			GameObject.Find ("GameManager").SendMessage("GameOver", SendMessageOptions.DontRequireReceiver);
		}
	}


	void DamageEnd()  {
		_anim.SetBool ("damageChk", false);
	}


	void damageSound()  {
		_Start_GM.audio.PlayOneShot (_Start_GM._dammEffect);
	}



}
