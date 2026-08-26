import { useEffect, useState } from 'react'

function WorkOrdersPage() {

  const [workOrders, setWorkOrders] = useState([])
  const [error, setError] = useState('')

  useEffect(() => {

    fetch(`${import.meta.env.VITE_API_URL}/api/work-orders`)
      .then((response) => {

        if (!response.ok) {
          throw new Error('Failed to load data.')
        }

        return response.json()
      })
      .then((data) => {
        setWorkOrders(data)
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
      <h1>Work Orders</h1>

      <table>
        <thead>
          <tr>
            <th>ID</th>
            <th>Product Code</th>
            <th>Target</th>
            <th>Produced</th>
            <th>Defect</th>
            <th>Status</th>
            <th>Equipment</th>
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
              <td>{workOrder.status}</td>
              <td>{workOrder.equipmentId}</td>
            </tr>
          ))}
        </tbody>
      </table>
    </>
  )
}

export default WorkOrdersPage