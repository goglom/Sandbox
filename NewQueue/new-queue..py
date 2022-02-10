import random


def shuffle(list):
    gen = random.Random(0)
    gen.shuffle(list)


class Student:
    def __init__(self, name: str) -> None:
        self.name = name
        self.credits = 0

    def __repr__(self) -> str:
        return f'<{self.name}: {self.credits}>'

    def __eq__(self, __o: str) -> bool:
        return __o == self.name


class StudentsQueue:
    def __init__(self, *args) -> None:
        self.students = []
        for name in args:
            self.students.append(Student(name))

    def make_table(self) -> dict[int, str]:
        table = {}
        for student in self.students:
            if not student.credits in table:
                table[student.credits] = []
            
            table[student.credits].append(student)
        
        return table

    def run_epoch(self) -> None:
        """
            Хочется разбить студентов на категории по количеству кредитов.
            Если в группе больше одного студента, то их необходимо перемешать.
            И далее составить список из студентов в порядке уменьшения числа их кредитов. 
            Если студенты имеют одинаковое число scocial credits то их случайное размещение уже было обеспеено на 
            пердыдущем шаге.
            После формирования списка (очереди, то есть) им необходимо заплать social credits в размере их индекса очереди
            (т.е. первый студент получит 0))), второй 1 и т.д)
        """
        table = self.make_table()

        for students in table.values():
            if len(students) > 1:
                shuffle(students)

        self.students = []
        for _, students in sorted(table.items(), key=lambda x: x[0], reverse=True):
            for student in students:
                self.students.append(student)


        for i, student in enumerate(self.students):
            student.credits += i


names = ('Vadim', 'Fedya', 'Vanya', 'Stepan')

queue = StudentsQueue(*names)


medium_pos = {
    'Vadim': 0.,
    'Fedya': 0., 
    'Vanya': 0.,
    'Stepan': 0.
}

N = 20

for i in range(N):
    for name in names:
        medium_pos[name] += queue.students.index(name)
    
    if i == 10:
        queue.students[-1], queue.students[1] = queue.students[1], queue.students[-1]

    queue.run_epoch()
    print(queue.students)

for name in medium_pos:
    medium_pos[name] /= N

print(medium_pos)