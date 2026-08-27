import { useEffect, useState } from 'react'

function EquipmentsPage() {
  const [equipments, setEquipments] = useState([])
  const [error, setError] = useState('')

  useEffect(() => {
    fetch(`${import.meta.env.VITE_API_URL}/api/equipments`)
      .then((response) => {
        if (!response.ok) throw new Error('Failed to load data.')
        return response.json()
      })
      .then((data) => setEquipments(data))
      .catch(() => setError('Failed to load data.'))
  }, [])

  if (error) {
    return <div className="message">{error}</div>
  }

  return (
    <>
      <section className="page-heading">
        <div>
          <p className="eyebrow">Manufacturing Monitoring System</p>
          <h1 className="page-title">Equipments</h1>
          <p className="page-subtitle">Monitor registered equipment and production status.</p>
        </div>
      </section>

      <section className="panel page-panel">
        <div className="panel-header">
          <h2 className="panel-title">Equipment List</h2>
          <p className="table-summary">{equipments.length} equipments</p>
        </div>

        <div className="table-scroll">
          <table className="data-table">
            <thead>
              <tr>
                <th>ID</th><th>Name</th><th>Status</th><th>Produced</th><th>Defect</th>
              </tr>
            </thead>
            <tbody>
              {equipments.map((equipment) => (
                <tr key={equipment.id}>
                  <td>{equipment.id}</td>
                  <td>{equipment.name}</td>
                  <td>
                    <span className={`status-badge ${equipment.status.toLowerCase()}`}>
                      {equipment.status}
                    </span>
                  </td>
                  <td>{equipment.totalProduced}</td>
                  <td>{equipment.totalDefect}</td>
                </tr>
              ))}
            </tbody>
          </table>
        </div>
      </section>
    </>
  )
}

export default EquipmentsPage
