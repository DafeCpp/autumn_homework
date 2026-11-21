import functools
import http.server
import json
from pathlib import Path
import subprocess
import tempfile


def validate(source):
    lines = source.strip().splitlines()
    if not lines:
        raise ValueError("Введи n m и строки зависимостей.")

    n, m = map(int, lines[0].split())
    if not 1 <= n <= 40 or not 0 <= m <= 300:
        raise ValueError("Для анимации введи от 1 до 40 вершин и до 300 рёбер.")
    if len(lines) != m + 1:
        raise ValueError(f"После первой строки должно быть ровно {m} пар.")

    edges = []
    for line in lines[1:]:
        a, b = map(int, line.split())
        if not (0 <= a < n and 0 <= b < n):
            raise ValueError(f"Номера вершин должны быть от 0 до {n - 1}.")
        edges.append((a, b))
    return n, edges


class Handler(http.server.SimpleHTTPRequestHandler):
    def __init__(self, *args, binary, directory, **kwargs):
        self.binary = binary
        super().__init__(*args, directory=directory, **kwargs)

    def reply(self, status, payload):
        body = json.dumps(payload, ensure_ascii=False).encode("utf-8")
        self.send_response(status)
        self.send_header("Content-Type", "application/json; charset=utf-8")
        self.send_header("Content-Length", str(len(body)))
        self.end_headers()
        self.wfile.write(body)

    def do_POST(self):
        if self.path != "/check":
            self.send_error(404)
            return

        try:
            length = int(self.headers.get("Content-Length", "0"))
            if not 0 < length <= 16384:
                raise ValueError("Введи граф размером до 16 КБ.")
            source = self.rfile.read(length).decode("utf-8")
            n, edges = validate(source)
        except ValueError as error:
            self.reply(400, {"error": str(error)})
            return

        try:
            answer = subprocess.run(
                [str(self.binary)], input=source + "\n", text=True,
                capture_output=True, check=True, timeout=5,
            ).stdout.strip()

            if answer != "-1":
                order = list(map(int, answer.split()))
                position = {vertex: i for i, vertex in enumerate(order)}
                if (len(order) != n or set(order) != set(range(n))
                        or any(position[b] >= position[a] for a, b in edges)):
                    raise ValueError("C++ вывел недопустимый порядок установки.")
        except (subprocess.SubprocessError, ValueError) as error:
            self.reply(500, {"error": str(error)})
            return

        self.reply(200, {"answer": answer})


def main():
    here = Path(__file__).resolve().parent
    with tempfile.TemporaryDirectory() as directory:
        binary = Path(directory) / "installer"
        subprocess.run(
            ["g++", "-std=c++23", "-Wall", "-Wextra",
             str(here.parent / "src" / "main.cpp"), "-o", str(binary)],
            check=True,
        )

        handler = functools.partial(Handler, binary=binary, directory=here)
        with http.server.ThreadingHTTPServer(("127.0.0.1", 8765), handler) as server:
            print("Открой Ports → 8765 → Open in Browser", flush=True)
            server.serve_forever()


if __name__ == "__main__":
    main()