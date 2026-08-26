import { BrowserRouter, Routes, Route, Link } from 'react-router-dom'
import DashboardPage from './pages/DashboardPage'
import EquipmentsPage from './pages/EquipmentsPage'
import WorkOrdersPage from './pages/WorkOrdersPage'
import LotsPage from './pages/LotsPage'
import './App.css'

function App() {
  return (
    <BrowserRouter>

      <nav className="navbar">
        <h2 className="logo">FactoryFlow</h2>

        <div className="nav-links">
          <Link to="/">Dashboard</Link>
          <Link to="/equipments">Equipments</Link>
          <Link to="/work-orders">Work Orders</Link>
          <Link to="/lots">Lots</Link>
        </div>
      </nav>

      <div className="dashboard">
        <Routes>
          <Route path="/" element={<DashboardPage />} />
          <Route path="/equipments" element={<EquipmentsPage />} />
          <Route path="/work-orders" element={<WorkOrdersPage />} />
          <Route path="/lots" element={<LotsPage />} />
        </Routes>
      </div>

    </BrowserRouter>
  )
}

export default App