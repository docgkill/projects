using UnityEngine;
using System.Collections;

public class PlayerColliderListener : MonoBehaviour {

	private PlayerStateListener _targetStateListener;
	public GameObject _player;

	void Start()  {
		_targetStateListener = (PlayerStateListener)_player.GetComponent<PlayerStateListener>();
	}

	void OnTriggerEnter2D( Collider2D collidedObject )  {
		switch(collidedObject.tag)
		{
		case "Platform":
			// When the player lands on a platform, toggle the Landing state.
			_targetStateListener.onStateChange(PlayerStateController.
			                                  playerStates.landing);
			break;
		} }
}
