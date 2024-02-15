from SimulatedAnnealing import _simulated_annealing


class tsp:
    def __init__(self, distance_matrix):
        """
        :param distance_matrix: A square matrix representing the distances between cities.
                                distance_matrix[i][j] is the distance between city i and city j
        """
        self.distance_matrix = distance_matrix
        self.num_of_cities = len(self.distance_matrix)
    
 
    def simulated_annealing(self):
        """
        :return: Returns the best tour found and the cost of that tour
                 A tour is represented using path representation
        """
        return _simulated_annealing(self.num_of_cities,
                                    self.distance_matrix)

