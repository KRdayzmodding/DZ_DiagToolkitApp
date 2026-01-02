#pragma once

#include <AUI/Platform/AWindow.h>
#include <AUI/Util/Declarative/Containers.h>
#include <AUI/Util/UIBuildingHelpers.h>
#include <AUI/View/ALabel.h>
#include <AUI/View/AButton.h>
#include <AUI/Platform/APlatform.h>
#include <AUI/Platform/AProcess.h>
#include <AUI/View/ADrawableView.h>
#include <AUI/View/AProgressBar.h>
#include <AUI/View/Dynamic.h>
#include "MyUpdater.h"
#include <print>
#include "macro.h"

using namespace declarative;

class MainWindow : public AWindow {
public:
    MainWindow(_<MyUpdater> updater) : AWindow("Project template app", 300_dp, 200_dp), mUpdater(std::move(updater)) {
        setContents(Centered { Vertical {
          Centered { Icon { ":img/icon.svg" } AUI_OVERRIDE_STYLE { FixedSize(64_dp) } },
          Centered { Label { "Hello world from AUI!" } },
          _new<AButton>("Visit GitHub repo")
              .connect(&AView::clicked, this, [] { APlatform::openUrl("https://github.com/aui-framework/aui"); }),
          _new<AButton>("Visit docs")
              .connect(&AView::clicked, this, [] { APlatform::openUrl("https://aui-framework.github.io/"); }),
          _new<AButton>("Submit an issue")
              .connect(
                  &AView::clicked, this, [] { APlatform::openUrl("https://github.com/aui-framework/aui/issues/new"); }),

          AUI_DYNAMIC({
              const std::any& status = mUpdater->status;

              if (const auto* idle = std::any_cast<AUpdater::StatusIdle>(&status)) {
                  return _new<AButton>("Check for updates")
                      .connect(&AView::clicked, AUI_SLOT(mUpdater)::checkForUpdates);
              }
              if (const auto* checking = std::any_cast<AUpdater::StatusCheckingForUpdates>(&status)) {
                  return _<AView>(_new<ALabel>("Checking for updates..."));
              }
              if (const auto* downloading = std::any_cast<AUpdater::StatusDownloading>(&status)) {
                  return Vertical {
                      Label { "Downloading..." },
                      _new<AProgressBar>() & downloading->progress,
                  };
              }
              if (const auto* waiting = std::any_cast<AUpdater::StatusWaitingForApplyAndRestart>(&status)) {
                  return _new<AButton>("Apply update and restart")
                      .connect(&AView::clicked, AUI_SLOT(mUpdater)::applyUpdateAndRestart);
              }

              return nullptr;
          }),
          Label { "Btw, 2 + 2 = {}"_format(sum(2, 2)) },
          Label { "Version: " AUI_PP_STRINGIZE(AUI_CMAKE_PROJECT_VERSION) },
        } });
    }

    static int sum(int a, int b) { return a + b; }

private:
    _<MyUpdater> mUpdater;
};
