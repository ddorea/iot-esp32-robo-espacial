from flask import Flask, request, jsonify
import sqlite3

app = Flask(__name__)

# ======================================
# CONEXÃO BANCO
# ======================================

def conectar():

    return sqlite3.connect("leituras.db")

# ======================================
# CRIA TABELA
# ======================================

with conectar() as conn:

    conn.execute("""
    CREATE TABLE IF NOT EXISTS leituras (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        timestamp TEXT,
        temperatura_c REAL,
        umidade_pct REAL,
        luminosidade INTEGER,
        presenca INTEGER,
        probabilidade_vida REAL
    )
    """)

# ======================================
# POST /leituras
# ======================================

@app.route("/leituras", methods=["POST"])
def salvar_leitura():

    dados = request.json

    with conectar() as conn:

        conn.execute("""
        INSERT INTO leituras (
            timestamp,
            temperatura_c,
            umidade_pct,
            luminosidade,
            presenca,
            probabilidade_vida
        )
        VALUES (?, ?, ?, ?, ?, ?)
        """, (
            dados["timestamp"],
            dados["temperatura_c"],
            dados["umidade_pct"],
            dados["luminosidade"],
            dados["presenca"],
            dados["probabilidade_vida"]
        ))

    return jsonify({
        "mensagem": "Leitura salva!"
    }), 201

# ======================================
# GET /leituras
# ======================================

@app.route("/leituras", methods=["GET"])
def listar_leituras():

    with conectar() as conn:

        cursor = conn.execute("""
        SELECT * FROM leituras
        ORDER BY id DESC
        LIMIT 100
        """)

        dados = cursor.fetchall()

    return jsonify(dados)

# ======================================
# MAIN
# ======================================

if __name__ == "__main__":

    app.run(debug=True)