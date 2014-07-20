using UnityEngine;
using System.Collections;


public class CameraController : MonoBehaviour {
	public PlayerStateController.playerStates _currentPlayerState = PlayerStateController.playerStates.idle;
	public GameObject _playerObject = null;
	public float _cameraTrackingSpeed = 0.2f;
	private Vector3 _lastTargetPosition = Vector3.zero;
	private Vector3 _currTargetPosition = Vector3.zero;
	private float _currLerpDistance = 0.0f;

	// Use this for initialization
	void Start () {
		// what difference between localPosition and position
		Vector3 playerPos = _playerObject.transform.position;
		Vector3 cameraPos = transform.position;
		Vector3 startTargetPos = playerPos;

		startTargetPos.z = cameraPos.z;
		_lastTargetPosition = startTargetPos;
		_currTargetPosition = startTargetPos;
		_currLerpDistance = 1.0f;

	
	}
	
	// Update is called once per frame
	void Update () {
	
	}

	void OnEable()  {
		PlayerStateController.onStateChange += onPlayerStateChange;
	}


	void OnDisable()  {
		PlayerStateController.onStateChange -= onPlayerStateChange;
	}


	void onPlayerStateChange(PlayerStateController.playerStates newState)  {
		_currentPlayerState = newState;
	}

	void LateUpdate()
	{
		// Update based on our current state
		onStateCycle();
		// Continue moving to the current target position
		_currLerpDistance += _cameraTrackingSpeed;
		transform.position = Vector3.Lerp(_lastTargetPosition,
		                                  _currTargetPosition, _currLerpDistance);
	}

	// Every cycle of the engine, process the current state
	void onStateCycle()  {				
		/*
		 * We use the player state to determine the current action that the camera should take. 
		 * Notice that in most cases we are tracking the player 
		 * - however, in the case of killing or resurrecting, 
		 * we don't want to track the player.
		 */
		switch (_currentPlayerState) {
			case PlayerStateController.playerStates.idle:
				trackPlayer ();
			break;
			case PlayerStateController.playerStates.left:
				trackPlayer ();
			break;
			case PlayerStateController.playerStates.right:
				trackPlayer();
			break;
			
		}
	}

	void trackPlayer()  {
		// Get and store the current camera position, and the current
		// player position, in world coordinates
		Vector3 currCamPos = transform.position;
		Vector3 currPlayerPos = _playerObject.transform.position;
		if(currCamPos.x == currPlayerPos.x && currCamPos.y == currPlayerPos.y)  {
			// Positions are the same - tell the camera not to move, then abort.
			_currLerpDistance = 1f;
			_lastTargetPosition = currCamPos;
			_currTargetPosition = currCamPos;
			return;
		}
		// Reset the travel distance for the lerp
		_currLerpDistance = 0f;
		// Store the current target position so we can lerp from it
		_lastTargetPosition = currCamPos;
		// Store the new target position
		_currTargetPosition = currPlayerPos;
		// Change the Z position of the target to the same as the current.
		//We don' want that to change.
		_currTargetPosition.z = currCamPos.z;
	}

	void stopTrackingPlayer()  {
		// Set the target positioning to the camera's current position
		// to stop its movement in its tracks
		Vector3 currCamPos = transform.position;
		_currTargetPosition = currCamPos;
		_lastTargetPosition = currCamPos;
		// Also set the lerp progress distance to 1.0f, which will tell the lerping that it is finished.
		// Since we set the target positionins to the camera's current position, the camera will just
		// lerp to its current spot and stop there.
		_currLerpDistance = 1.0f;
	}
}

