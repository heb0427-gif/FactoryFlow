import { useEffect, useState } from 'react'

const POLLING_SECONDS = 5

function DashboardPage() {
  const [summary, setSummary] = useState(null)
  const [alarms, setAlarms] = useState([])
  const [error, setError] = useState('')
  const [secondsLeft, setSecondsLeft] = useState(POLLING_SECONDS)
  const [lastUpdated, setLastUpdated] = useState(null)

  const fetchSummary = () => {
    return fetch(`${import.meta.env.VITE_API_URL}/api/summary`)
      .then((response) => {
        if (!response.ok) {
          throw new Error('Failed to load data.')
        }
        return response.json()
      })
      .then((data) => {
        setSummary(data)
      })
  }

  const fetchAlarms = () => {
    return fetch(`${import.meta.env.VITE_API_URL}/api/alarms`)
      .then((response) => {
        if (!response.ok) {
          throw new Error('Failed to load alarms.')
        }
        return response.json()
      })
      .then((data) => {
        setAlarms(data)
      })
  }

  const fetchDashboard = () => {
    setError('')

    Promise.all([fetchSummary(), fetchAlarms()])
      .then(() => {
        setLastUpdated(new Date())
        setSecondsLeft(POLLING_SECONDS)
      })
      .catch(() => {
        setError('Failed to load dashboard data.')
      })
  }

  useEffect(() => {
    fetchDashboard()

    const pollingId = setInterval(() => {
      fetchDashboard()
    }, POLLING_SECONDS * 1000)

    const countdownId = setInterval(() => {
      setSecondsLeft((current) => current <= 1 ? POLLING_SECONDS : current - 1)
    }, 1000)

    return () => {
      clearInterval(pollingId)
      clearInterval(countdownId)
    }
  }, [])

  if (error && !summary) {
    return <div className="message">{error}</div>
  }

  if (!summary) {
    return <div className="message">Loading dashboard...</div>
  }

  const formattedLastUpdated = lastUpdated
    ? lastUpdated.toLocaleString()
    : 'Waiting for first update...'

  const metrics = [
    { label: 'Total Produced', value: summary.totalProduced, icon: '◆', tone: 'green' },
    { label: 'Total Defect', value: summary.totalDefect, icon: '!', tone: 'red' },
    { label: 'Defect Rate', value: `${summary.defectRate.toFixed(2)}%`, icon: '%', tone: 'purple' },
    { label: 'Equipment Count', value: summary.equipmentCount, icon: '▦', tone: 'blue' },
    { label: 'Alarm Count', value: summary.alarmCount, icon: '♧', tone: 'orange' },
  ]

  return (
    <>
      <section className="page-heading">
        <div>
          <p className="eyebrow">Manufacturing Monitoring System</p>
          <h1 className="page-title">FactoryFlow Dashboard</h1>
          <p className="page-subtitle">Live manufacturing data. Reliable operations.</p>
        </div>

        <div className="polling-panel">
          <div
            className="polling-ring"
            key={lastUpdated?.getTime()}
            aria-hidden="true"
          />
          <div className="polling-copy">
            <p className="polling-label">Next update in {secondsLeft}s</p>
            <p className="polling-caption">(5s polling)</p>
          </div>
          <div className="last-updated">
            Last updated
            <strong>{formattedLastUpdated}</strong>
          </div>
        </div>
      </section>

      <div className="dashboard-status-row">
        <span className="status-pill">
          <span className="status-dot" />
          API Connected
        </span>
        <span>Dashboard refreshes automatically every 5 seconds.</span>
      </div>

      <section className="metric-grid">
        {metrics.map((metric) => (
          <article className="metric-card" key={metric.label}>
            <div className="metric-card-top">
              <div className={`metric-icon ${metric.tone}`}>{metric.icon}</div>
              <p className="metric-label">{metric.label}</p>
            </div>
            <p className="metric-value">{metric.value}</p>
          </article>
        ))}
      </section>

      <section className="dashboard-content-grid">
        <article className="panel system-card">
          <h2>System Status</h2>
          <div className="system-list">
            <div className="system-item">
              <span className="system-name">REST API</span>
              <span className="system-value"><span className="status-dot" />Connected</span>
            </div>
            <div className="system-item">
              <span className="system-name">Polling</span>
              <span className="system-value"><span className="status-dot" />Active · 5 sec</span>
            </div>
            <div className="system-item">
              <span className="system-name">Last Sync</span>
              <span className="system-value">
                {lastUpdated ? lastUpdated.toLocaleTimeString() : '-'}
              </span>
            </div>
          </div>
        </article>

        <article className="panel alarms-panel">
          <div className="panel-header">
            <h2 className="panel-title">Recent Alarms</h2>
            <span className="panel-meta">{alarms.length} records</span>
          </div>

          <div className="table-scroll">
            <table className="data-table">
              <thead>
                <tr>
                  <th>Time</th>
                  <th>Equipment</th>
                  <th>Temperature</th>
                  <th>Message</th>
                </tr>
              </thead>
              <tbody>
                {alarms.length === 0 ? (
                  <tr>
                    <td colSpan="4" className="empty-state">No alarms detected.</td>
                  </tr>
                ) : (
                  alarms.map((alarm) => (
                    <tr key={alarm.id}>
                      <td>{alarm.createdAt}</td>
                      <td>{alarm.equipmentId}</td>
                      <td className="danger-text">{alarm.temperature}</td>
                      <td>{alarm.message}</td>
                    </tr>
                  ))
                )}
              </tbody>
            </table>
          </div>
        </article>
      </section>
    </>
  )
}

export default DashboardPage
