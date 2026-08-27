import { useState } from 'react'
import { BrowserRouter, NavLink, Routes, Route } from 'react-router-dom'
import DashboardPage from './pages/DashboardPage'
import EquipmentsPage from './pages/EquipmentsPage'
import WorkOrdersPage from './pages/WorkOrdersPage'
import LotsPage from './pages/LotsPage'
import './App.css'

function App() {
  const [menuOpen, setMenuOpen] = useState(false)

  const closeMenu = () => {
    setMenuOpen(false)
  }

  return (
    <BrowserRouter>
      <header className="app-header">
        <div className="header-inner">
          <NavLink to="/" className="brand" onClick={closeMenu}>
            <span className="brand-mark" aria-hidden="true">▥</span>
            <span>FactoryFlow</span>
          </NavLink>

          <button
            className="menu-toggle"
            type="button"
            aria-label="Toggle navigation"
            aria-expanded={menuOpen}
            onClick={() => setMenuOpen((open) => !open)}
          >
            <span />
            <span />
            <span />
          </button>

          <nav className={`nav-links ${menuOpen ? 'is-open' : ''}`}>
            <NavLink to="/" end onClick={closeMenu}
              className={({ isActive }) => isActive ? 'nav-link active' : 'nav-link'}>
              <span className="nav-icon" aria-hidden="true">▣</span>
              Dashboard
            </NavLink>

            <NavLink to="/equipments" onClick={closeMenu}
              className={({ isActive }) => isActive ? 'nav-link active' : 'nav-link'}>
              <span className="nav-icon" aria-hidden="true">⚙</span>
              Equipments
            </NavLink>

            <NavLink to="/work-orders" onClick={closeMenu}
              className={({ isActive }) => isActive ? 'nav-link active' : 'nav-link'}>
              <span className="nav-icon" aria-hidden="true">▤</span>
              Work Orders
            </NavLink>

            <NavLink to="/lots" onClick={closeMenu}
              className={({ isActive }) => isActive ? 'nav-link active' : 'nav-link'}>
              <span className="nav-icon" aria-hidden="true">⬡</span>
              Lots
            </NavLink>
          </nav>
        </div>
      </header>

      <main className="page-shell">
        <Routes>
          <Route path="/" element={<DashboardPage />} />
          <Route path="/equipments" element={<EquipmentsPage />} />
          <Route path="/work-orders" element={<WorkOrdersPage />} />
          <Route path="/lots" element={<LotsPage />} />
        </Routes>
      </main>
    </BrowserRouter>
  )
}

export default App
