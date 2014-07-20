using UnityEngine;
using System.Collections;

public class Enemy : MonoBehaviour {

	public GameObject[] _enemySetObj;
	public float _speed;


	// Use this for initialization
	void Start () {
		_enemySetObj [0].transform.localPosition += new Vector3 (0, Random.Range (-2, 3) * 130.0f, 0f);
		_enemySetObj [1].transform.localPosition += new Vector3 (0, Random.Range (-2, 3) * 130.0f, 0f);
		_enemySetObj [2].transform.localPosition += new Vector3 (0, Random.Range (-2, 3) * 130.0f, 0f);
		_enemySetObj [3].transform.localPosition += new Vector3 (0, Random.Range (-2, 3) * 130.0f, 0f);
	}
	
	// Update is called once per frame
	void Update () {
		transform.Translate (_speed * Time.deltaTime, 0, 0);
	}
}
