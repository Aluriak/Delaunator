/* Create the TrianguledObject, used by Delaunator's Users */
%pythoncode %{




class TrianguledObject(object):
        """
        Object defined by Delaunator module for simplify using of it.
        This is like an abstract class : no real code need to have pure Trianguled Object defined.
        Correct use is to use objects that inherits from this class.
        """
        VRTL_VRTX_TO_TRNGLD_OBJCT = {}


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



# ACCESSORS ###################################################################
        def directNeighbors(self):
                return self.virtual_vertex.directNeighbors()



# PREDICAT ####################################################################
        def inTriangulation(self):
                """
                @return True iff associated with a triangulation.
                """
                return self.virtual_vertex is not None



# CLASS METHOD ################################################################
        @staticmethod
        def of(virtual_vertex):
                return TrianguledObject.VRTL_VRTX_TO_TRNGLD_OBJCT[virtual_vertex.id()]



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
        if not isinstance(coords, Coordinates):
                coords = Coordinates(*coords)
        self.movVirtualVertex(tri_obj.virtual_vertex, coords)
Delaunator.movTrianguledObject = movTrianguledObject





%}








