install(
    TARGETS MassHunterLibToQuant_exe
    RUNTIME COMPONENT MassHunterLibToQuant_Runtime
)

if(PROJECT_IS_TOP_LEVEL)
  include(CPack)
endif()
