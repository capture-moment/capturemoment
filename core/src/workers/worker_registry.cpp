/**
 * @file worker_registry.cpp
 * @brief Implementation of WorkerRegistry.
 * @author CaptureMoment Team
 * @date 2026
 */

#include "workers/worker_registry.h"
#include "workers/halide/halide_operation_worker.h"

#include <spdlog/spdlog.h>

namespace CaptureMoment::Core::Workers {

void WorkerRegistry::registerAll(WorkerBuilder& builder)
{
    spdlog::info("[WorkerRegistry::registerAll]: Registering all workers");

    registerHalideWorkers(builder);
    // registerAIWorkers(builder); // TODO: Enable when AI worker is ready

    spdlog::info("[WorkerRegistry::registerAll]: All workers registered");
}

void WorkerRegistry::registerHalideWorkers(WorkerBuilder& builder)
{
    spdlog::debug("[WorkerRegistry::registerHalideWorkers]: Registering Halide workers");

    builder.registerCreator(WorkerType::HalideOperation, []() {
        return std::make_unique<HalideOperationWorker>();
    });
    spdlog::trace("[WorkerRegistry::registerHalideWorkers]: Registered HalideOperationWorker");
}

void WorkerRegistry::registerAIWorkers([[maybe_unused]] WorkerBuilder& builder)
{
    spdlog::debug("[WorkerRegistry::registerAIWorkers]: Registering AI/Computer Vision workers");

    // Exemple: Sky Replacement
    // builder.registerCreator(WorkerType::SkyAI, []() {
    //     return std::make_unique<SkyReplacementWorker>();
    // });
    // spdlog::trace("WorkerRegistry: Registered SkyReplacementWorker");
}

} // namespace CaptureMoment::Core::Workers
