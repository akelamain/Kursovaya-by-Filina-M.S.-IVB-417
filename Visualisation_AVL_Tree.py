import tkinter as tk
from tkinter import messagebox


# --- 1. Логика AVL Дерева ---

class Node:
    def __init__(self, key):
        self.key = key
        self.left = None
        self.right = None
        self.height = 1


class AVLTree:
    def get_height(self, node):
        if not node:
            return 0
        return node.height

    def get_balance(self, node):
        if not node:
            return 0
        return self.get_height(node.left) - self.get_height(node.right)

    def rotate_right(self, y):
        x = y.left
        T2 = x.right
        x.right = y
        y.left = T2
        y.height = 1 + max(self.get_height(y.left), self.get_height(y.right))
        x.height = 1 + max(self.get_height(x.left), self.get_height(x.right))
        return x

    def rotate_left(self, x):
        y = x.right
        T2 = y.left
        y.left = x
        x.right = T2
        x.height = 1 + max(self.get_height(x.left), self.get_height(x.right))
        y.height = 1 + max(self.get_height(y.left), self.get_height(y.right))
        return y

    def insert(self, root, key):
        # 1. Обычная вставка в BST
        if not root:
            return Node(key)
        elif key < root.key:
            root.left = self.insert(root.left, key)
        else:
            root.right = self.insert(root.right, key)

        # 2. Обновление высоты
        root.height = 1 + max(self.get_height(root.left), self.get_height(root.right))

        # 3. Проверка баланса
        balance = self.get_balance(root)

        # 4. Балансировка (4 случая)
        # Left Left
        if balance > 1 and key < root.left.key:
            return self.rotate_right(root)
        # Right Right
        if balance < -1 and key > root.right.key:
            return self.rotate_left(root)
        # Left Right
        if balance > 1 and key > root.left.key:
            root.left = self.rotate_left(root.left)
            return self.rotate_right(root)
        # Right Left
        if balance < -1 and key < root.right.key:
            root.right = self.rotate_right(root.right)
            return self.rotate_left(root)

        return root


# --- 2. Визуализация и Интерфейс (Tkinter) ---

class AVLVisualizer:
    def __init__(self, root_window):
        self.root_window = root_window
        self.root_window.title("Визуализация AVL Дерева")
        self.root_window.geometry("800x600")

        self.tree = AVLTree()
        self.root_node = None

        # Настройка UI
        self.setup_ui()

    def setup_ui(self):
        # Панель ввода
        frame = tk.Frame(self.root_window, pady=10)
        frame.pack()

        tk.Label(frame, text="Введите число:").pack(side=tk.LEFT)
        self.entry = tk.Entry(frame, width=10)
        self.entry.pack(side=tk.LEFT, padx=5)
        self.entry.focus_set()
        self.entry.bind("<Return>", lambda event: self.add_node())  # Enter работает как кнопка

        tk.Button(frame, text="Добавить", command=self.add_node, bg="#4CAF50", fg="white").pack(side=tk.LEFT, padx=5)
        tk.Button(frame, text="Очистить", command=self.clear_tree, bg="#f44336", fg="white").pack(side=tk.LEFT)

        # Холст для рисования
        self.canvas = tk.Canvas(self.root_window, width=800, height=500, bg="white")
        self.canvas.pack(fill=tk.BOTH, expand=True)

    def add_node(self):
        try:
            value = int(self.entry.get())
            self.root_node = self.tree.insert(self.root_node, value)
            self.entry.delete(0, tk.END)
            self.draw_tree()
        except ValueError:
            messagebox.showwarning("Ошибка", "Введите целое число!")

    def clear_tree(self):
        self.root_node = None
        self.canvas.delete("all")

    # Расчет координат для узлов
    def calculate_positions(self, node, depth, x_counter):
        if node is None:
            return x_counter

        # Идем влево
        x_counter = self.calculate_positions(node.left, depth + 1, x_counter)

        # Текущий узел
        # X зависит от порядка (in-order traversal), Y от глубины
        self.positions[node] = (x_counter * 60 + 50, depth * 80 + 50)
        x_counter += 1

        # Идем вправо
        x_counter = self.calculate_positions(node.right, depth + 1, x_counter)
        return x_counter

    def draw_tree(self):
        self.canvas.delete("all")
        self.positions = {}

        if not self.root_node:
            return

        # 1. Считаем позиции
        self.calculate_positions(self.root_node, 0, 0)

        # 2. Рисуем связи (линии)
        self.draw_edges(self.root_node)

        # 3. Рисуем узлы и текст
        for node, (x, y) in self.positions.items():
            balance = self.tree.get_balance(node)

            # Цвет узла: Зеленый (сбалансирован), Красный (несбалансирован - теоретически на миг)
            color = "#e1f5fe"
            if abs(balance) > 1:
                color = "#ffcdd2"  # Красноватый если дисбаланс

            # Рисуем круг
            r = 25
            self.canvas.create_oval(x - r, y - r, x + r, y + r, fill=color, outline="black", width=2)

            # Текст ключа
            self.canvas.create_text(x, y, text=str(node.key), font=("Arial", 12, "bold"))

            # Текст баланса (внизу узла)
            self.canvas.create_text(x, y + 35, text=f"BF:{balance}", fill="gray", font=("Arial", 10))

    def draw_edges(self, node):
        if not node:
            return

        px, py = self.positions[node]

        if node.left:
            lx, ly = self.positions[node.left]
            self.canvas.create_line(px, py, lx, ly, width=2)
            self.draw_edges(node.left)

        if node.right:
            rx, ry = self.positions[node.right]
            self.canvas.create_line(px, py, rx, ry, width=2)
            self.draw_edges(node.right)


# --- Запуск программы ---
if __name__ == "__main__":
    root = tk.Tk()
    app = AVLVisualizer(root)
    root.mainloop()