import { useState } from 'react'
import './App.css'

function App() {
  const[ form, setForm] = useState({
    fullname: "",
    username: "",
    phoneno: "",
    birthday: "",
    location: "", 
    email: "",
    password: "",});
 
  return (
    <div style={{ textAlign: "center", marginTop: "50px" }}>
      <h1>Welcome to My React Page!</h1>
      <p>This is my first simple React app</p>
    </div>
  );
}

export default App;