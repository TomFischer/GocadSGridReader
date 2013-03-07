SET ( SOURCES
	main.cpp
	GocadSGridReader.cpp
)

SET ( HEADERS
	GocadSGridReader.h
)
	

INCLUDE_DIRECTORIES(
	.
	../ThirdParty/
)

# Create the executable
ADD_EXECUTABLE( ReadGoCadPoints
        ${SOURCES}
)

