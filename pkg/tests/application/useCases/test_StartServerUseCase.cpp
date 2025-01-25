#include "application/dto/StartServerDTO.hpp"
#include "application/useCases/StartServerUseCase.hpp"

#include <gtest/gtest.h>

const std::pair<int, std::string> portPassword = std::make_pair(8080, "password");

TEST(StartServerUseCaseTest, TestInitialization) {
  StartServerDTO dto(portPassword);
  EXPECT_NO_THROW({ StartServerUseCase useCase(dto); });
}

TEST(StartServerUseCaseTest, TestExecute) {
  StartServerDTO dto(portPassword);
  StartServerUseCase useCase(dto);
  EXPECT_NO_THROW({ useCase.execute(); });
}

TEST(StartServerUseCaseTest, TestCleanup) {
  StartServerDTO dto(portPassword);
  StartServerUseCase *useCase = new StartServerUseCase(dto);
  delete useCase;
}
