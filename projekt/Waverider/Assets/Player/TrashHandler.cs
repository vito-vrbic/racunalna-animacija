using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UIElements;

public class TrashHandler : MonoBehaviour
{
    public static TrashHandler Instance;

    public int TrashPickedUp { get; private set; } = 0;

    public int MaximumActiveTrash = 10;
    public float TrashSpawnChance = 0.001f;
    public GameObject TrashPrefab;

    private int _ActiveTrash = 0;

    // Define perimeter for spawning
    public float SpawnRadius = 50f;

    void Awake()
    {
        if (Instance == null) Instance = this;
        else Destroy(this.gameObject);
    }

    void Update()
    {
        StartCoroutine(SpawnRoutine());
    }

    IEnumerator SpawnRoutine()
    {
        while (true)
        {
            yield return new WaitForSeconds(1f);

            if (_ActiveTrash + TrashPickedUp < MaximumActiveTrash)
            {
                if (Random.value < TrashSpawnChance)
                {
                    SpawnTrash();
                }
            }
        }
    }

    public void DestroyedActive() 
    {
        _ActiveTrash = Mathf.Max(_ActiveTrash - 1, 0);
    }

    void SpawnTrash()
    {
        // Pick a random angle around the circle.
        float angle = Random.Range(0f, 360f) * Mathf.Deg2Rad;

        // Compute position on perimeter.
        float x = Mathf.Cos(angle) * SpawnRadius;
        float z = Mathf.Sin(angle) * SpawnRadius;
        Vector3 spawnPos = transform.position + new Vector3(x, 0f, z);

        // Spawn the trash.
        GameObject trash = Instantiate(TrashPrefab, spawnPos, Quaternion.identity);
        _ActiveTrash++;
    }

    private void OnTriggerEnter(Collider other)
    {
        if (other.CompareTag("Barrel"))
        {
            TrashPickedUp++;
            Destroy(other.transform.parent.gameObject);
        }
    }
}
