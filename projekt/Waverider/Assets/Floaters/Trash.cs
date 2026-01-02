using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Trash : MonoBehaviour
{
    private GameObject _Player;
    // Start is called before the first frame update
    void Start()
    {
        _Player = TrashHandler.Instance.gameObject;
        
    }

    // Update is called once per frame
    void Update()
    {
        if (Vector3.Distance(transform.position, _Player.transform.position) > TrashHandler.Instance.SpawnRadius) Destroy(gameObject);
    }

    private void OnDestroy()
    {
        TrashHandler.Instance.DestroyedActive();
    }
}
