import { useEffect, useState } from 'react'

function EquipmentsPage() {

  const [equipments, setEquipments] = useState([])
  const [error, setError] = useState('')

  useEffect(() => {

    fetch(`${import.meta.env.VITE_API_URL}/api/equipments`)
      .then((response) => {

        if (!response.ok) {
          throw new Error('Failed to load data.')
        }

        return response.json()
      })
      .then((data) => {
        setEquipments(data)
      })
      .catch(() => {
        setError('Failed to load data.')
      })

  }, [])

  if (error) {
    return <div className="message">{error}</div>
  }

  return (
    <>
      <h1>Equipments</h1>

      <table>
        <thead>
          <tr>
            <th>ID</th>
            <th>Name</th>
            <th>Status</th>
            <th>Produced</th>
            <th>Defect</th>
          </tr>
        </thead>

        <tbody>
          {equipments.map((equipment) => (
            <tr key={equipment.id}>
              <td>{equipment.id}</td>
              <td>{equipment.name}</td>
              <td>{equipment.status}</td>
              <td>{equipment.totalProduced}</td>
              <td>{equipment.totalDefect}</td>
            </tr>
          ))}
        </tbody>
      </table>
    </>
  )
}

export default EquipmentsPage