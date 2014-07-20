using UnityEngine;
using System.Collections;

public class GameManager : MonoBehaviour {

	public GameObject _enemySet;
	public GameObject _nearBgObj;
	public Transform _playerObjPool;

	public bool _spawnChk = true;

	public GameObject _result;
	public UILabel _resultText;

	public UILabel _scoreText;

	public float _timerForLevel = 0;
	public float _timerForLevelLimit = 10.0f;




	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {

		_timerForLevel = _timerForLevel + Time.deltaTime;
		if(_timerForLevel > _timerForLevelLimit)  {
			if(Time.timeScale < 5f)  {
				Time.timeScale *= Time.timeScale;
				_timerForLevelLimit *= _timerForLevelLimit;
			}
			_timerForLevel = 0f;
		}


		if(_nearBgObj.transform.localPosition.x < -2460.0f && _spawnChk )  {

			var set1 = Instantiate (_enemySet, Vector3.zero, Quaternion.identity) as GameObject;
			set1.transform.parent = _playerObjPool;
			set1.transform.localScale = new Vector3 (1, 1, 1);
			set1.transform.localPosition = Vector3.zero;
			_spawnChk = false;
		}

		if (_nearBgObj.transform.localPosition.x > -1300.0f && !_spawnChk) {
				
			_spawnChk = true;

		}




	}

	public void myResume() {
		Time.timeScale = 1.0f;
		_result.SetActive (false);
		Application.LoadLevel ("1_play");

	}

	void GameOver()  {
		_result.SetActive (true);
		_resultText.text = "score is " + _scoreText.text;
		Time.timeScale = 0.0f;
	}

}
