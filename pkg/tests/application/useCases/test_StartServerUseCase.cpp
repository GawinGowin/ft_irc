#include "application/dto/StartServerDTO.hpp"
#include "application/useCases/StartServerUseCase.hpp"

#include <gtest/gtest.h>

const int argc = 3;
char *argv[] = {(char *)"./ft_irc", (char *)"8080", (char *)"password", nullptr};

TEST(StartServerUseCaseTest, TestInitialization) {
  StartServerDTO dto(argc, argv);
  EXPECT_NO_THROW({ StartServerUseCase useCase(dto); });
}

TEST(StartServerUseCaseTest, TestExecute) {
  StartServerDTO dto(argc, argv);
  StartServerUseCase useCase(dto);
  EXPECT_NO_THROW({ useCase.execute(); });
}

TEST(StartServerUseCaseTest, TestCleanup) {
  StartServerDTO dto(argc, argv);
  StartServerUseCase *useCase = new StartServerUseCase(dto);
  delete useCase;
}
