from graphviz import Digraph

def draw_vector_diagram(circuit_vector, name = "circuit diagram"):
    tailing = max(circuit_vector)
    concentrate = tailing - 1
    feed = circuit_vector[0]

    g = Digraph(name)

    g.attr(rankdir='LR')
    g.attr('node', shape='rectangle')

    g.edge('Feed', str(feed), color='blue', headport='w', tailport='e', arrowhead='normal', arrowtail='normal')

    for unit in range(int((len(circuit_vector)-1)/2)):
        g.edge(str(unit), str(circuit_vector[(unit+1)*2-1]), headport='w', tailport='s', arrowhead='normal', color='blue')
        g.edge(str(unit), str(circuit_vector[(unit+1)*2]), headport='w', tailport='n', arrowhead='normal', color='red')
    
    g.edge(str(concentrate), "concentrate", color='blue')
    g.edge(str(tailing), "tailing", color='red')
    
    g.render(filename=name, cleanup=True, format='png')
    
    return 1

if __name__ == '__main__':
    name = "circuit diagram"
    circuit_vector = [3, 7, 1, 3, 2, 6, 11, 7, 11, 8, 0, 4, 7, 5, 2, 6, 1, 9, 1, 10, 7]    
    draw_vector_diagram(circuit_vector, name)