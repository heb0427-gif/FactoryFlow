import { useEffect, useState } from 'react'

function DashboardPage() {

  const [summary, setSummary] = useState(null)
  const [alarms, setAlarms] = useState([])
  const [error, setError] = useState('')

  const fetchSummary = () => {
    fetch(`${import.meta.env.VITE_API_URL}/api/summary`)
    .then((response) => {
      if (!response.ok) {
        throw new Error('Failed to load data.')
      }

      return response.json()
    })
    .then((data) => {
      setSummary(data)
    })
    .catch(() => {
      setError('Failed to load data.')
    })
  }

  const fetchAlarms = () => {
    fetch(`${import.meta.env.VITE_API_URL}/api/alarms`)
      .then((response) => {
        if (!response.ok) {
          throw new Error('Failed to load alarms.')
        }

      return response.json()
    })
    .then((data) => {
      setAlarms(data)
    })
    .catch(() => {
      setError('Failed to load alarms.')
    })
  }

  useEffect(() => {

    // 페이지 처음 열었을 때 즉시 조회
    fetchSummary()
    fetchAlarms()

    // 이후 5초마다 다시 조회
    const intervalId = setInterval(() => {
      fetchSummary()
      fetchAlarms()
    }, 5000)

    // DashboardPage가 사라지면 interval 제거
    return () => {
      clearInterval(intervalId)
    }

  }, [])

  if (error) {
    return <div className="message">{error}</div>
  }

  if (!summary) {
    return <div className="message">Loading...</div>
  }

  return (
    <>
      <h1>FactoryFlow Dashboard</h1>

      <div className="card-container">

        <div className="card">
          <h2>Total Produced</h2>
          <p>{summary.totalProduced}</p>
        </div>

        <div className="card">
          <h2>Total Defect</h2>
          <p>{summary.totalDefect}</p>
        </div>

        <div className="card">
          <h2>Defect Rate</h2>
          <p>{summary.defectRate.toFixed(2)}%</p>
        </div>

        <div className="card">
          <h2>Equipment Count</h2>
          <p>{summary.equipmentCount}</p>
        </div>

        <div className="card">
          <h2>Alarm Count</h2>
          <p>{summary.alarmCount}</p>
        </div>

      </div>

      <h2 className="section-title">Recent Alarms</h2>

      <table>
        <thead>
          <tr>
            <th>Time</th>
            <th>Equipment</th>
            <th>Temperature</th>
            <th>Message</th>
          </tr>
        </thead>

        <tbody>
          {alarms.map((alarm) => (
            <tr key={alarm.id}>
              <td>{alarm.createdAt}</td>
              <td>{alarm.equipmentId}</td>
              <td>{alarm.temperature}</td>
              <td>{alarm.message}</td>
            </tr>
          ))} 
        </tbody>
      </table>

    </>
  )
}

export default DashboardPage