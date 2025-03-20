#include "application/dto/StartServerDTO.hpp"
#include "application/serviceLocator/ConfigsServiceLocator.hpp"
#include "application/useCases/StartServerUseCase.hpp"
#include "domain/configs/Configs.hpp"

#include <gtest/gtest.h>

const int argc = 3;
char *argv[] = {(char *)"./ft_irc", (char *)"8085", (char *)"password", nullptr};

// テスト用の設定で初期化する
void setupTestConfigs() {
  ConfigsLoader &loader = ConfigsServiceLocator::get();
  Configs configs = loader.getConfigs(); // 現在の設定をコピー

  configs.Limits.MaxConnections = 10;
  configs.Limits.MaxBufferSize = 1024;

  loader.setConfigs(configs);
}

TEST(StartServerUseCaseTest, TestInitialization) {
  setupTestConfigs();
  StartServerDTO dto(argc, argv);
  EXPECT_NO_THROW({ StartServerUseCase useCase(dto); });
}

TEST(StartServerUseCaseTest, TestExecute) {
  setupTestConfigs();
  StartServerDTO dto(argc, argv);
  StartServerUseCase useCase(dto);
  EXPECT_NO_THROW({ useCase.execute(); });
}

TEST(StartServerUseCaseTest, TestCleanup) {
  setupTestConfigs();
  StartServerDTO dto(argc, argv);
  StartServerUseCase *useCase = new StartServerUseCase(dto);
  delete useCase;
}
