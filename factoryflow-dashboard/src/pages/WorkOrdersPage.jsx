import { useEffect, useState } from 'react'

function WorkOrdersPage() {
  const [workOrders, setWorkOrders] = useState([])
  const [error, setError] = useState('')

  useEffect(() => {
    fetch(`${import.meta.env.VITE_API_URL}/api/work-orders`)
      .then((response) => {
        if (!response.ok) throw new Error('Failed to load data.')
        return response.json()
      })
      .then((data) => setWorkOrders(data))
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
          <h1 className="page-title">Work Orders</h1>
          <p className="page-subtitle">
            Review production targets, results, and assigned equipment.
          </p>
        </div>
      </section>

      <section className="panel page-panel">
        <div className="panel-header">
          <h2 className="panel-title">Work Order List</h2>
          <p className="table-summary">{workOrders.length} work orders</p>
        </div>

        <div className="table-scroll">
          <table className="data-table">
            <thead>
              <tr>
                <th>ID</th><th>Product Code</th><th>Target</th><th>Produced</th>
                <th>Defect</th><th>Status</th><th>Equipment</th>
              </tr>
            </thead>
            <tbody>
              {workOrders.map((workOrder) => (
                <tr key={workOrder.id}>
                  <td>{workOrder.id}</td>
                  <td>{workOrder.productCode}</td>
                  <td>{workOrder.targetQuantity}</td>
                  <td>{workOrder.producedQuantity}</td>
                  <td>{workOrder.defectQuantity}</td>
                  <td>
                    <span className={`status-badge ${workOrder.status.toLowerCase()}`}>
                      {workOrder.status}
                    </span>
                  </td>
                  <td>{workOrder.equipmentId}</td>
                </tr>
              ))}
            </tbody>
          </table>
        </div>
      </section>
    </>
  )
}

export default WorkOrdersPage
