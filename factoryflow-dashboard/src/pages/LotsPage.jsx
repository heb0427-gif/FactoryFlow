import { useEffect, useState } from 'react'

function LotsPage() {

  const [lots, setLots] = useState([])
  const [error, setError] = useState('')

  useEffect(() => {

    fetch(`${import.meta.env.VITE_API_URL}/api/lots`)
      .then((response) => {

        if (!response.ok) {
          throw new Error('Failed to load data.')
        }

        return response.json()
      })
      .then((data) => {
        setLots(data)
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
      <h1>Lots</h1>

      <table>
        <thead>
          <tr>
            <th>ID</th>
            <th>Item Code</th>
            <th>Type</th>
            <th>Quantity</th>
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
    </>
  )
}

export default LotsPage