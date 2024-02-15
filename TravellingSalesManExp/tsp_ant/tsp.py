
from AntColony import _ant_colony


class tsp:
    def __init__(self, distance_matrix):
        """
        :param distance_matrix: A square matrix representing the distances between cities.
                                distance_matrix[i][j] is the distance between city i and city j
        """
        self.distance_matrix = distance_matrix
        self.num_of_cities = len(self.distance_matrix)
    

    def ant_colony(self,
                   num_of_ants=20,
                   pheromone_evapouration=0.2):
        """
        :param num_of_ants:            Number of ants in the colony
        :param pheromone_evapouration: Evapouration rate of the pheromone deposited by ants
        :return:                       Returns the best tour found and the cost of that tour
                                       A tour is represented using path representation
        """
        return _ant_colony(self.num_of_cities,
                           self.distance_matrix,
                           num_of_ants,
                           pheromone_evapouration)
