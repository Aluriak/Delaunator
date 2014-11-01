/* Create the TrianguledObject, used by Delaunator's Users */
%pythoncode %{




class TrianguledObject(object):
        """
        Object defined by Delaunator module for simplify using of it.
        This is like an abstract class : no real code need to have pure Trianguled Object defined.
        Correct use is to use objects that inherits from this class.
        """
        VRTL_VRTX_TO_TRNGLD_OBJCT = {None:None}


# CONSTRUCTOR #################################################################
        def __init__(self, delaunator = None, coords = None):
                """
                @param delaunator a valid reference to a delaunay triangulation, or None (default is None)
                @param coords (x,y) tuple, position in the triangulation, or None (default is None)
                @note that self will attach itself if and only if given args are not None, and coords are valid in delaunator
                """
                if delaunator is not None:
                        delaunator.addTrianguledObject(self, coords)



# PUBLIC METHOD ###############################################################
        def coordinates(self):
                return self.virtual_vertex.coordinates()


        def squareDistanceTo(self, tri_obj):
                """
                @param tri_obj a trianguled object instance
                @return square distance to this object, according to coordinates of self and tri_obj
                @warning the distance returned is the squared distance. Useful for compare distances without slow call to sqrt()
                """
                return self.coordinates().squareDistanceTo(tri_obj.coordinates())


        def distanceTo(self, tri_obj):
                """
                @param tri_obj a trianguled object instance
                @return distance to this object, according to coordinates of self and tri_obj
                """
                return self.coordinates().distanceTo(tri_obj.coordinates())


# ACCESSORS ###################################################################
        @property
        def virtualConfundedNeighbors(self):
                """
                @return list of VirtualVertex, that are confunded neighbors of self
                @note confunded neighbors share the same Vertex instance.
                """
                return (_ for _ in self.virtual_vertex.confundedNeighbors() if self.virtual_vertex.id() !=  vv.id())


        @property
        def confundedNeighbors(self):
                """
                @return list of TrianguledObject, that are confunded neighbors of self
                @note confunded neighbors share the same Vertex instance.
                """
                return (TrianguledObject.of(_) for _ in self.virtual_vertex.confundedNeighbors() if self.virtual_vertex.id() !=  vv.id())


        @property
        def directNeighbors(self):
                """
                @return list of direct neighbors of self
                @note direct neighbors have no sense out of mathematical definition of delaunay triangulation, because its just objects that are directly connected to self by an edge.
                """
                return self.virtual_vertex.directNeighbors()


        def nearerNeighbors(self, nb_neighbors, confounded = False):
                """
                @param nb_neighbors a positiv integer
                @param confounded a boolean (False by default). If true, confounded TrianguledObject count for only one, but are all returned.
                @return list that contain the nb_neighbors neighbors in nearer-first order
                """
                return (TrianguledObject.of(_) for _ in self.virtual_vertex.nearerNeighbors(nb_neighbors, confounded) if self.virtual_vertex.id() !=  vv.id())


        def neighborsAt(self, max_distance, min_distance = 0):
                """
                @param max_distance number that give the limit distance 
                @param min_distance number that give the low limit. No low limit if equal to zero (default value is zero)
                @return list that contain the TrianguledObject that are at max_distance at most of self 
                """
                return (TrianguledObject.of(vv) for vv in self.virtual_vertex.neighborsAt(max_distance, min_distance) if self.virtual_vertex.id() !=  vv.id())



# PREDICAT ####################################################################
        def inTriangulation(self):
                """
                @return True iff associated with a triangulation.
                """
                return self.virtual_vertex is not None



# CLASS METHOD ################################################################
        @staticmethod
        def of(virtual_vertex):
                return TrianguledObject.VRTL_VRTX_TO_TRNGLD_OBJCT[virtual_vertex.id()] if virtual_vertex is not None else None



%}







/* Extending Delaunator class for pythonic and useful methods access */
%pythoncode %{




def trianguledObjects(self):
        """
        @return iterable of objects
        """
        return (TrianguledObject.of(_) for _ in self.virtualVertices())
Delaunator.trianguledObjects = trianguledObjects




def addTrianguledObject(self, tri_obj, coords):
        """
        @param tri_obj TrianguledObject to add
        @param coords (x,y) where tri_obj will be place
        @return the id of tri_obj in self instance, or None iff coords invalid 
        """
        added = None
        # transform user coords into Coordinates object
        if not isinstance(coords, Coordinates):
                coords = Coordinates(*coords)
        # add the trianguled object
        if tri_obj is not None and coords is not None:
                tri_obj.virtual_vertex  = self.addVirtualVertex(coords)
                if tri_obj.virtual_vertex is not None:
                        # all is ok, object is added !
                        TrianguledObject.VRTL_VRTX_TO_TRNGLD_OBJCT[tri_obj.virtual_vertex.id()] = tri_obj
                        added = tri_obj.virtual_vertex.id()
        return added
Delaunator.addTrianguledObject = addTrianguledObject




def delTrianguledObject(self, tri_obj):
        """
        @param tri_obj the TrianguledObject that will be forget
        Detach tri_obj of self. Destroy associated VirtualVertex.
        """
        if tri_obj.virtual_vertex is not None:
                # delete entry in dictionnary
                del TrianguledObject.VRTL_VRTX_TO_TRNGLD_OBJCT[tri_obj.virtual_vertex.id()]
                self.delVirtualVertex(tri_obj.virtual_vertex)
                tri_obj.virtual_vertex = None
Delaunator.delTrianguledObject = delTrianguledObject
       



def movTrianguledObject(self, tri_obj, coords):
        """
        @param tri_obj the TrianguledObject that will be moved
        @param coords that will be added to current tri_obj coordinates
        """
        # transform user coords into Coordinates object
        if not isinstance(coords, Coordinates):
                coords = Coordinates(*coords)
        # do the move
        self.movVirtualVertex(tri_obj.virtual_vertex, coords)
Delaunator.movTrianguledObject = movTrianguledObject





%}








