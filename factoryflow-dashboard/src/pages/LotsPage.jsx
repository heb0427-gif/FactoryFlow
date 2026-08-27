import { useEffect, useState } from 'react'

function LotsPage() {
  const [lots, setLots] = useState([])
  const [error, setError] = useState('')

  useEffect(() => {
    fetch(`${import.meta.env.VITE_API_URL}/api/lots`)
      .then((response) => {
        if (!response.ok) throw new Error('Failed to load data.')
        return response.json()
      })
      .then((data) => setLots(data))
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
          <h1 className="page-title">Lots</h1>
          <p className="page-subtitle">
            Track material and product lots across the production flow.
          </p>
        </div>
      </section>

      <section className="panel page-panel">
        <div className="panel-header">
          <h2 className="panel-title">LOT List</h2>
          <p className="table-summary">{lots.length} lots</p>
        </div>

        <div className="table-scroll">
          <table className="data-table">
            <thead>
              <tr>
                <th>ID</th><th>Item Code</th><th>Type</th><th>Quantity</th>
              </tr>
            </thead>
            <tbody>
              {lots.map((lot) => (
                <tr key={lot.id}>
                  <td>{lot.id}</td>
                  <td>{lot.itemCode}</td>
                  <td>{lot.type}</td>
                  <td>{lot.quantity}</td>
                </tr>
              ))}
            </tbody>
          </table>
        </div>
      </section>
    </>
  )
}

export default LotsPage
