library(shiny)
library(ggplot2)
library(dplyr)
library(plotly)
library(rsconnect)
#ran in console 
#rsconnect::deployApp('/Users/mariagarcia/Desktop/DAT301/lab3')

ui <- fluidPage(
  titlePanel("Lab 3: Probability Distributions and Scatter Plot Visualization"),
  
  sidebarLayout(
    sidebarPanel(
      selectInput("dist", "Choose Your Distrobution:", 
                  choices = c("Normal", "Uniform", "Exponential")),
      sliderInput("param1", "Parameter 1:", min = 0, max = 10, value = 5),
      sliderInput("param2", "Parameter 2:", min = 0, max = 10, value = 2),
      checkboxInput("showSummary", "Show Summary Statistics", value = TRUE),
      
      hr(),
      h4("Scatter Plot Parameters"),
      sliderInput("mean1", "Mean of X:", min = 0, max = 10, value = 5),
      sliderInput("sd1", "Standard Deviation of X:", min = 0, max = 5, value = 1),
      sliderInput("mean2", "Mean of Y:", min = 0, max = 10, value = 5),
      sliderInput("sd2", "Standard Deviation of Y:", min = 0, max = 5, value = 1)
    ),
    
    mainPanel(
      plotlyOutput("distPlot"),
      tableOutput("summaryTable"),
      plotlyOutput("scatterPlot")
    )
  )
)


server <- function(input, output) {
  
  distData <- reactive({
    dist <- input$dist
    param1 <- input$param1
    param2 <- input$param2
    
    if (dist == "Normal") {
      data <- rnorm(1000, mean = param1, sd = param2)
    } else if (dist == "Uniform") {
      data <- runif(1000, min = param1, max = param2)
    } else if (dist == "Exponential") {
      data <- rexp(1000, rate = param1)
    }
    
    data
  })
  
  output$distPlot <- renderPlotly({
    data <- distData()
    dist <- input$dist
    x_label <- if (dist == "Normal") {
      "Observed Value"
    } else if (dist == "Uniform") {
      "Uniformly Distributed Value"
    } else if (dist == "Exponential") {
      "Time"
    } else {
      "Value"
    }
  
  p <- ggplot(data.frame(x = data), aes(x)) +
    geom_density(fill = "forestgreen", alpha = 0.5) +
    ggtitle(paste(input$dist, "Distribution")) +
    xlab(x_label) +
    ylab("Density") +
    theme_minimal()
    
    ggplotly(p)
  })
  
  output$summaryTable <- renderTable({
    if (input$showSummary) {
      data <- distData()
      summary <- data.frame(
        Statistic = c("Mean", "Standard Deviation", "Min", "Max"),
        Value = c(mean(data), sd(data), min(data), max(data))
      )
      summary
    }
  })
  
  scatterData <- reactive({
    mean1 <- input$mean1
    sd1 <- input$sd1
    mean2 <- input$mean2
    sd2 <- input$sd2
    
    x <- rnorm(1000, mean = mean1, sd = sd1)
    y <- rnorm(1000, mean = mean2, sd = sd2)
    
    data.frame(x = x, y = y)
  })
  
  output$scatterPlot <- renderPlotly({
    data <- scatterData()
    p <- ggplot(data, aes(x = x, y = y)) +
      geom_point(alpha = 0.5) +
      ggtitle("Scatter Plot using Random Data Points") +
      theme_minimal()
    
    ggplotly(p)
  })
}

shinyApp(ui = ui, server = server)

