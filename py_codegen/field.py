
from collections import namedtuple

FieldImpl = namedtuple('Field', ['type', 'name'])


class Field(FieldImpl):
    def declaration(self):
        return self.type + " " + self.name + ";"
