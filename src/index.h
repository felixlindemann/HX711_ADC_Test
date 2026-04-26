#ifndef INDEX_H
#define INDEX_H

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="de">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Smart Scale Dashboard</title>
    <link href="https://fonts.googleapis.com/css2?family=Outfit:wght@300;400;600&display=swap" rel="stylesheet">
    <style>
        :root {
            --primary: #00f2ff;
            --bg: #0f172a;
            --card-bg: rgba(30, 41, 59, 0.7);
            --text: #f8fafc;
        }

        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
            font-family: 'Outfit', sans-serif;
        }

        body {
            background: radial-gradient(circle at top right, #1e293b, #0f172a);
            color: var(--text);
            min-height: 100vh;
            display: flex;
            justify-content: center;
            align-items: center;
            padding: 20px;
            overflow: hidden;
        }

        /* Glassmorphism Card */
        .dashboard {
            background: var(--card-bg);
            backdrop-filter: blur(12px);
            -webkit-backdrop-filter: blur(12px);
            border: 1px solid rgba(255, 255, 255, 0.1);
            border-radius: 24px;
            padding: 40px;
            width: 100%;
            max-width: 450px;
            box-shadow: 0 25px 50px -12px rgba(0, 0, 0, 0.5);
            text-align: center;
            position: relative;
        }

        .dashboard::before {
            content: '';
            position: absolute;
            top: -2px;
            left: -2px;
            right: -2px;
            bottom: -2px;
            background: linear-gradient(45deg, transparent, rgba(0, 242, 255, 0.3), transparent);
            border-radius: 26px;
            z-index: -1;
        }

        h1 {
            font-weight: 300;
            letter-spacing: 2px;
            text-transform: uppercase;
            font-size: 0.9rem;
            color: var(--primary);
            margin-bottom: 30px;
            opacity: 0.8;
        }

        .weight-container {
            margin: 40px 0;
        }

        #weight {
            font-size: 5rem;
            font-weight: 600;
            text-shadow: 0 0 30px rgba(0, 242, 255, 0.4);
            transition: all 0.3s ease;
        }

        .unit {
            font-size: 1.5rem;
            font-weight: 300;
            margin-left: 10px;
            opacity: 0.6;
        }

        .status {
            font-size: 0.8rem;
            color: #94a3b8;
            margin-bottom: 40px;
            display: flex;
            justify-content: center;
            align-items: center;
            gap: 8px;
        }

        .status-dot {
            width: 8px;
            height: 8px;
            background: #10b981;
            border-radius: 50%;
            box-shadow: 0 0 8px #10b981;
            animation: pulse 2s infinite;
        }

        @keyframes pulse {
            0% { opacity: 1; transform: scale(1); }
            50% { opacity: 0.3; transform: scale(0.8); }
            100% { opacity: 1; transform: scale(1); }
        }

        .actions {
            display: flex;
            gap: 15px;
            justify-content: center;
        }

        button {
            background: rgba(255, 255, 255, 0.05);
            border: 1px solid rgba(255, 255, 255, 0.1);
            color: white;
            padding: 12px 24px;
            border-radius: 12px;
            cursor: pointer;
            font-size: 1rem;
            font-weight: 500;
            transition: all 0.3s cubic-bezier(0.4, 0, 0.2, 1);
            display: flex;
            align-items: center;
            gap: 10px;
        }

        button:hover {
            background: var(--primary);
            color: var(--bg);
            border-color: var(--primary);
            box-shadow: 0 0 20px rgba(0, 242, 255, 0.4);
            transform: translateY(-2px);
        }

        button:active {
            transform: translateY(0);
        }

        .tare-icon {
            font-size: 1.2rem;
        }

        /* Mobile tweaks */
        @media (max-width: 480px) {
            .dashboard {
                padding: 30px 20px;
            }
            #weight {
                font-size: 4rem;
            }
        }
    </style>
</head>
<body>
    <div class="dashboard">
        <h1>Smart Scale</h1>
        
        <div class="weight-container">
            <span id="weight">0.0</span><span class="unit">g</span>
        </div>

        <div class="status">
            <div class="status-dot"></div>
            <span id="connection">Verbunden</span>
        </div>

        <div class="actions">
            <button onclick="tareScale()">
                <span class="tare-icon">⚖️</span> TARA
            </button>
        </div>
    </div>

    <script>
        function tareScale() {
            fetch('/tare')
                .then(response => {
                    if (response.ok) {
                        console.log('Tariert');
                        const weightEl = document.getElementById('weight');
                        weightEl.style.opacity = '0.3';
                        setTimeout(() => weightEl.style.opacity = '1', 500);
                    }
                })
                .catch(err => console.error('Error:', err));
        }

        // SSE for real-time data
        if (!!window.EventSource) {
            const source = new EventSource('/events');

            source.addEventListener('open', function(e) {
                console.log("SSE Open");
                document.getElementById('connection').innerText = "Live";
                document.querySelector('.status-dot').style.background = "#10b981";
            }, false);

            source.addEventListener('error', function(e) {
                if (e.target.readyState != EventSource.OPEN) {
                    console.log("SSE Error");
                    document.getElementById('connection').innerText = "Verbindung unterbrochen";
                    document.querySelector('.status-dot').style.background = "#ef4444";
                }
            }, false);

            source.addEventListener('weight_update', function(e) {
                console.log("Weight:", e.data);
                const weight = parseFloat(e.data).toFixed(2);
                document.getElementById('weight').innerText = weight;
            }, false);
        }
    </script>
</body>
</html>
)rawliteral";

#endif
