#include "workerfactory.h"

#include "../tools/tools.h"
#include "alexarank/checkalexarankworker.h"
#include "checksearchengineindex/checksearchengineindexworker.h"
#include "dummy/dummyworker.h"
#include "scrapeemails/scrapeemailsworker.h"
#include "scrapeimages/scrapeimagesworker.h"
#include "scrapelinks/scrapelinksworker.h"
#include "scrapephonenumbers/scrapephonenumbersworker.h"
#include "scrapeproxies/scrapeproxiesworker.h"
#include "scrapesitemaps/scrapesitemapsworker.h"
#include "test/testworker.h"
#include "urlstatus/checkurlstatusworker.h"

// FIXME: This is temporary solution, switching tools/worker should be resolved differently
Worker* workerFactory(int id, int workerId, QQueue<QVariantMap>* inputDataQueue, QMutex* mutex, const QVariantMap& settings)
{
    Worker* worker(nullptr);

    switch (id) {
    case Tools::CHECK_URL_STATUS:
        worker = new CheckUrlStatusWorker(workerId, inputDataQueue, mutex, settings);
        break;
    case Tools::CHECK_ALEXA_RANK:
        worker = new CheckAlexaRankWorker(workerId, inputDataQueue, mutex, settings);
        break;
    case Tools::SCRAPE_PROXIES:
        worker = new ScrapeProxiesWorker(workerId, inputDataQueue, mutex, settings);
        break;
    case Tools::SCRAPE_LINKS:
        worker = new ScrapeLinkskWorker(workerId, inputDataQueue, mutex, settings);
        break;
    case Tools::SCRAPE_SITEMAPS:
        worker = new ScrapeSitemapskWorker(workerId, inputDataQueue, mutex, settings);
        break;
    case Tools::SCRAPE_EMAILS:
        worker = new ScrapeEmailsWorker(workerId, inputDataQueue, mutex, settings);
        break;
    case Tools::SCRAPE_PHONE_NUMBERS:
        worker = new ScrapePhoneNumbersWorker(workerId, inputDataQueue, mutex, settings);
        break;
    case Tools::CHECK_SEARCH_ENGINE_INDEX:
        worker = new CheckSearchEngineIndexWorker(workerId, inputDataQueue, mutex, settings);
        break;
    case Tools::SCRAPE_IMAGES:
        worker = new ScrapeImagesWorker(workerId, inputDataQueue, mutex, settings);
        break;
    case Tools::TEST:
        worker = new TestWorker(workerId, inputDataQueue, mutex, settings);
        break;
    default:
        worker = new DummyWorker(workerId, inputDataQueue, mutex, settings);
    }

    return worker;
}
